open Js.Promise;

open Knex;

open KnexUtils;

open Schema;

type t = {
  /* Queries */
  getAll: (~size: option(PaperClip.size)) => Js.Promise.t(array(PaperClip.t)),
  getById: (~id: string) => Js.Promise.t(PaperClip.t),
  /* Mutations */
  add: (~paperClip: input) => Js.Promise.t(PaperClip.t),
  update: (~id: string, ~paperClip: input) => Js.Promise.t(PaperClip.t),
  remove: (~id: string) => Js.Promise.t({. "success": bool})
};

let handleResponse:
  (~error: string, array(Json.PaperClip.t)) => Js.Promise.t(Js.Array.t(PaperClip.t)) =
  rejectIfEmpty(~decoder=Decode.paperClip);

let getAll = (paperClips: Knex.queryBuilder, ~size) => {
  let query =
    switch size {
    | Some(size) => paperClips |> where({"size": PaperClip.sizeToJs(size)})
    | None => paperClips
    };
  query |> select("*") |> toPromise |> then_(handleResponse(~error="No PaperClips found."))
};

let getById = (paperClips: Knex.queryBuilder, ~id) =>
  paperClips
  |> where({"id": id})
  |> select("*")
  |> toPromise
  |> then_(handleResponse(~error="No PaperClip found with id: " ++ id))
  |> then_(pickFirst)
  |> handleDbErrors;

let add = (paperClips: Knex.queryBuilder, ~paperClip) =>
  paperClips
  |> insert(paperClip)
  |> returning("*")
  |> toPromise
  |> then_(handleResponse(~error="Unable to add PaperClip."))
  |> then_(pickFirst)
  |> handleDbErrors;

let update = (paperClips: Knex.queryBuilder, ~id, ~paperClip) =>
  paperClips
  |> where({"id": id})
  |> update(paperClip)
  |> toPromise
  |> then_(handleResponse(~error="Unable to update PaperClip with id: " ++ id))
  |> then_(pickFirst)
  |> handleDbErrors;

let remove = (paperClips: Knex.queryBuilder, ~id) =>
  paperClips
  |> where({"id": id})
  |> del()
  |> toPromise
  |> then_(
       (response) =>
         switch response {
         | [||] => reject(PromiseUtils.makeError("Unable to remove PaperClip with id: " ++ id))
         | _results => resolve({"success": true})
         }
     );

/**
 * Initialize a new PaperClipService
 */
let make = (postgres: PostgresProvider.t) => {
  let paperClips = postgres.fromTable(~name="paper_clips");
  {
    getAll: getAll(paperClips),
    getById: getById(paperClips),
    add: add(paperClips),
    update: update(paperClips),
    remove: remove(paperClips)
  }
};
