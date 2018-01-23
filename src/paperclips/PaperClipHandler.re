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

type t('root) = {
  resolvers,
  queries: {
    .
    "allPaperClips":
      (Js.Nullable.t('root), {. "filter": Js.Nullable.t(PaperClip.t)}, Js.t(graphQLContext)) =>
      Js.Promise.t(array(PaperClip.t)),
    "paperClip":
      (Js.Nullable.t('root), {. "id": string}, Js.t(graphQLContext)) => Js.Promise.t(PaperClip.t)
  },
  mutations: {
    .
    "addPaperClip":
      (Js.Nullable.t('root), {. "paperClip": PaperClip.t}, Js.t(graphQLContext)) =>
      Js.Promise.t(PaperClip.t),
    "updatePaperClip":
      (Js.Nullable.t('root), {. "id": string, "paperClip": PaperClip.t}, Js.t(graphQLContext)) =>
      Js.Promise.t(PaperClip.t),
    "removePaperClip":
      (Js.Nullable.t('root), {. "id": string}, Js.t(graphQLContext)) => Js.Promise.t(emptyResult)
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
      "allPaperClips": (_root, input, _context) => {
        let opt = Js.Nullable.to_opt(input##filter);
        switch opt {
        | Some(filter) => service.getAll(~size=PaperClip.sizeFromJs(filter##size))
        | None => service.getAll(~size=None)
        }
      },
      "paperClip": (_root, input, _context) => service.getById(~id=input##id)
    },
    mutations: {
      "addPaperClip": (_root, input, _context) => service.add(~paperClip=input##paperClip),
      "updatePaperClip": (_root, input, _context) =>
        service.update(~id=input##id, ~paperClip=input##paperClip),
      "removePaperClip": (_root, input, _context) => service.remove(~id=input##id)
    }
  }
};
