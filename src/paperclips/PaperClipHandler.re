open Config;

open Schema;

open Utils;

type resolvers = {
  .
  "PaperClip": {
    .
    "id": PaperClip.t => string,
    "createdAt": PaperClip.t => Js.Date.t,
    "updatedAt": PaperClip.t => Js.Date.t,
    "size": PaperClip.t => string
  }
};

let resolvers: resolvers = {
  "PaperClip": {
    "id": (paperClip) =>
      paperClip##id |> Utils.getOrError(~error="A Paper Clip was returned with no id"),
    "createdAt": (paperClip) =>
      paperClip##created_at |> Utils.getOrError(~error="A Link was returned with no created_at"),
    "updatedAt": (paperClip) =>
      paperClip##updated_at |> Utils.getOrError(~error="A Link was returned with no updated_at"),
    "size": (paperClip) => paperClip##size
  }
};

type emptyResult = {. "success": bool};

type t = {
  resolvers,
  queries: {
    .
    "allPaperClips":
      (graphQLContext, {. "filter": Js.Nullable.t(PaperClip.t)}) =>
      Js.Promise.t(array(PaperClip.t)),
    "paperClip": (graphQLContext, {. "id": string}) => Js.Promise.t(PaperClip.t)
  },
  mutations: {
    .
    "addPaperClip": (graphQLContext, {. "paperClip": PaperClip.t}) => Js.Promise.t(PaperClip.t),
    "updatePaperClip":
      (graphQLContext, {. "id": string, "paperClip": PaperClip.t}) => Js.Promise.t(PaperClip.t),
    "removePaperClip": (graphQLContext, {. "id": string}) => Js.Promise.t(emptyResult)
  }
};

/**
 * Initialize a new PaperClipHandler
 */
let make = (dataProvider: DataProvider.t) => {
  let service = PaperClipService.make(dataProvider);
  {
    resolvers,
    queries: {
      "allPaperClips": (_context, input) => {
        let opt = Js.Nullable.to_opt(input##filter);
        switch opt {
        | Some(filter) => service.getAll(~size=PaperClip.sizeFromJs(filter##size))
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
