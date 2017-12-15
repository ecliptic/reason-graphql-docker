open Config;

open PaperClip.Resolve;

type resolvers = {
  .
  "PaperClip": {
    .
    "id": (graphQLContext, PaperClip.t) => string,
    "createdAt": (graphQLContext, PaperClip.t) => string,
    "updatedAt": (graphQLContext, PaperClip.t) => string,
    "size": (graphQLContext, PaperClip.t) => string
  }
};

let resolvers: resolvers = {
  "PaperClip": {
    "id": (_context, paperClip) => paperClip |> id,
    "createdAt": (_context, paperClip) => paperClip |> createdAt,
    "updatedAt": (_context, paperClip) => paperClip |> updatedAt,
    "size": (_context, paperClip) => paperClip |> size
  }
};

type emptyResult = {. "success": bool};

type t('resolvers, 'context) = {
  resolvers: 'resolvers,
  queries: {
    .
    "allPaperClips":
      (Js.t('context), {. "filter": Js.Nullable.t(PaperClip.paperClipInput)}) =>
      Js.Promise.t(array(PaperClip.t)),
    "paperClip": (Js.t('context), {. "id": string}) => Js.Promise.t(PaperClip.t)
  },
  mutations: {
    .
    "addPaperClip":
      (Js.t('context), {. "paperClip": PaperClip.paperClipInput}) => Js.Promise.t(PaperClip.t),
    "updatePaperClip":
      (Js.t('context), {. "id": string, "paperClip": PaperClip.paperClipInput}) =>
      Js.Promise.t(PaperClip.t),
    "removePaperClip": (Js.t('context), {. "id": string}) => Js.Promise.t(emptyResult)
  }
};

/**
 * Initialize a new PaperClipHandler
 */
let make = (dataProvider: DataProvider.t('queryRow)) => {
  let service = PaperClipService.make(dataProvider);
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
