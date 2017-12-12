open Js.Promise;

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
  rejectIfEmpty(~decoder=PaperClip.Decode.paperClip);

let handleGetAll = (paperClips: query('row), ~size) => {
  let query =
    switch size {
    | Some(size) => paperClips |> where({"size": PaperClip.Size.toString(size)})
    | None => paperClips
    };
  query |> select("*") |> then_(handleResponse(~error="No PaperClips found."))
};

let handleGetById = (paperClips: query('row), ~id) =>
  paperClips
  |> where({"id": id})
  |> select("*")
  |> then_(handleResponse(~error="No PaperClip found with id: " ++ id))
  |> then_(pickFirst);

let handleAdd = (paperClips: query('row), ~paperClip) =>
  paperClips
  |> insert(paperClip)
  |> returning("*")
  |> then_(handleResponse(~error="Unable to add PaperClip."))
  |> then_(pickFirst);

let handleUpdate = (paperClips: query('row), ~id, ~paperClip) =>
  paperClips
  |> where({"id": id})
  |> update(paperClip)
  |> then_(handleResponse(~error="Unable to update PaperClip with id: " ++ id))
  |> then_(pickFirst);

let handleRemove = (paperClips: query('row), ~id) =>
  paperClips
  |> where({"id": id})
  |> del()
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
let make = (dataProvider: DataProvider.t('row)) => {
  let paperClips = dataProvider.postgres.fromTable(~name="paper_clips");
  {
    getAll: handleGetAll(paperClips),
    getById: handleGetById(paperClips),
    add: handleAdd(paperClips),
    update: handleUpdate(paperClips),
    remove: handleRemove(paperClips)
  }
};
