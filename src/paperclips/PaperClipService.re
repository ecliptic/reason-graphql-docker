open Js.Promise;

open Knex;

open KnexUtils;

type t = {
  /* Queries */
  getAll: (~size: option(PaperClip.Size.t)) => Js.Promise.t(array(PaperClip.t)),
  getById: (~id: string) => Js.Promise.t(PaperClip.t),
  /* Mutations */
  add: (~paperClip: PaperClip.paperClipInput) => Js.Promise.t(PaperClip.t),
  update: (~id: string, ~paperClip: PaperClip.paperClipInput) => Js.Promise.t(PaperClip.t),
  remove: (~id: string) => Js.Promise.t({. "success": bool})
};

let handleResponse:
  (~error: string, array(PaperClip.paperClipJson)) => Js.Promise.t(Js.Array.t(PaperClip.t)) =
  rejectIfEmpty(~decoder=PaperClip.decode);

let handleGetAll = (paperClips: Knex.query, ~size) => {
  let query =
    switch size {
    | Some(size) => paperClips |> where({"size": PaperClip.Size.toString(size)})
    | None => paperClips
    };
  query |> select("*") |> toPromise |> then_(handleResponse(~error="No PaperClips found."))
};

let handleGetById = (paperClips: Knex.query, ~id) =>
  paperClips
  |> where({"id": id})
  |> select("*")
  |> toPromise
  |> then_(handleResponse(~error="No PaperClip found with id: " ++ id))
  |> then_(pickFirst)
  |> handleDbErrors;

let handleAdd = (paperClips: Knex.query, ~paperClip) =>
  paperClips
  |> insert(paperClip)
  |> returning("*")
  |> toPromise
  |> then_(handleResponse(~error="Unable to add PaperClip."))
  |> then_(pickFirst)
  |> handleDbErrors;

let handleUpdate = (paperClips: Knex.query, ~id, ~paperClip) =>
  paperClips
  |> where({"id": id})
  |> update(paperClip)
  |> toPromise
  |> then_(handleResponse(~error="Unable to update PaperClip with id: " ++ id))
  |> then_(pickFirst)
  |> handleDbErrors;

let handleRemove = (paperClips: Knex.query, ~id) =>
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
    getAll: handleGetAll(paperClips),
    getById: handleGetById(paperClips),
    add: handleAdd(paperClips),
    update: handleUpdate(paperClips),
    remove: handleRemove(paperClips)
  }
};
