open Config;

open Schema;

type resolvers = {
  .
  "PaperClip": {
    .
    "id": PaperClip.t => string,
    "createdAt": PaperClip.t => string,
    "updatedAt": PaperClip.t => string,
    "size": PaperClip.t => string
  }
};

let resolvers: resolvers = {
  "PaperClip": {
    "id": (paperClip) => paperClip.id,
    "createdAt": (paperClip) => paperClip.createdAt |> toISOString,
    "updatedAt": (paperClip) => paperClip.updatedAt |> toISOString,
    "size": (paperClip) => paperClip.size
  }
};

type emptyResult = {. "success": bool};

type t = {
  resolvers,
  queries: {
    .
    "allPaperClips":
      (graphQLContext, {. "filter": Js.Nullable.t(PaperClip.paperClipInput)}) =>
      Js.Promise.t(array(PaperClip.t)),
    "paperClip": (graphQLContext, {. "id": string}) => Js.Promise.t(PaperClip.t)
  },
  mutations: {
    .
    "addPaperClip":
      (graphQLContext, {. "paperClip": PaperClip.paperClipInput}) => Js.Promise.t(PaperClip.t),
    "updatePaperClip":
      (graphQLContext, {. "id": string, "paperClip": PaperClip.paperClipInput}) =>
      Js.Promise.t(PaperClip.t),
    "removePaperClip": (graphQLContext, {. "id": string}) => Js.Promise.t(emptyResult)
  }
};

/**
 * Initialize a new PaperClipHandler
 */
let make = (dataProvider: DataProvider.t) => {
  let service = PaperClipService.make(dataProvider.postgres);
  {
    resolvers,
    queries: {
      "allPaperClips": (_context, input) => {
        let opt = Js.Nullable.to_opt(input##filter);
        switch opt {
        | Some(filter) => service.getAll(~size=Some(PaperClip.Size.fromString(filter##size)))
        | None => service.getAll(~size=None)
        }
      },
      "paperClip": (_context, input) => service.getById(~id=input##id)
    },
    mutations: {
      "addPaperClip": (_context, input) => service.add(~paperClip=input##paperClip),
      "updatePaperClip": (_context, input) =>
        service.update(~id=input##id, ~paperClip=input##paperClip),
      "removePaperClip": (_context, input) => service.remove(~id=input##id)
    }
  }
};
