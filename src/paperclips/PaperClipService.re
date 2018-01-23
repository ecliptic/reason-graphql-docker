open Schema;

open Js.Promise;

open PromiseUtils;

open Knex;

type t = {
  /* Queries */
  getAll: (~size: option(PaperClip.size)) => Js.Promise.t(array(PaperClip.t)),
  getById: (~id: string) => Js.Promise.t(PaperClip.t),
  /* Mutations */
  add: (~paperClip: PaperClip.t) => Js.Promise.t(PaperClip.t),
  update: (~id: string, ~paperClip: PaperClip.t) => Js.Promise.t(PaperClip.t),
  remove: (~id: string) => Js.Promise.t({. "success": bool})
};

let getAll = (paperClips: Knex.queryBuilder, ~size) => {
  let query =
    switch size {
    | Some(size) => paperClips |> where({"size": PaperClip.sizeToJs(size)})
    | None => paperClips
    };
  query |> select("*") |> toPromise |> then_(rejectIfEmpty(~error="No PaperClips found."))
};

let getById = (paperClips: Knex.queryBuilder, ~id) =>
  paperClips
  |> where({"id": id})
  |> select("*")
  |> toPromise
  |> then_(rejectIfEmpty(~error="No PaperClip found with id: " ++ id))
  |> then_(pickFirst)
  |> KnexUtils.handleDbErrors;

let add = (paperClips: Knex.queryBuilder, ~paperClip) =>
  paperClips
  |> insert(paperClip)
  |> returning("*")
  |> toPromise
  |> then_(rejectIfEmpty(~error="Unable to add PaperClip."))
  |> then_(pickFirst)
  |> KnexUtils.handleDbErrors;

let update = (paperClips: Knex.queryBuilder, ~id, ~paperClip) =>
  paperClips
  |> where({"id": id})
  |> update(paperClip)
  |> toPromise
  |> then_(rejectIfEmpty(~error="Unable to update PaperClip with id: " ++ id))
  |> then_(pickFirst)
  |> KnexUtils.handleDbErrors;

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
let make = (dataProvider: DataProvider.t) => {
  let paperClips = dataProvider.postgres.knex |> Knex.fromTable("paper_clips");
  {
    getAll: getAll(paperClips),
    getById: getById(paperClips),
    add: add(paperClips),
    update: update(paperClips),
    remove: remove(paperClips)
  }
};
