open Js.Option;

open Js.Promise;

open Schema;

let dummy: PaperClip.t = {
  "id": Js.Nullable.return("test-paper-clip-1"),
  "created_at": Js.Nullable.return(Js.Date.makeWithYMD(~year=2018.0, ~month=1.0, ~date=2.0, ())),
  "updated_at": Js.Nullable.return(Js.Date.makeWithYMD(~year=2018.0, ~month=1.0, ~date=2.0, ())),
  "size": "SMALL"
};

module Service = {
  let make =
      (
        ~getAll as mockGetAll,
        ~getById as mockGetById,
        ~add as mockAdd,
        ~update as mockUpdate,
        ~remove as mockRemove
      ) =>
    PaperClipService.{
      getAll: mockGetAll |> getWithDefault((~size as _size) => resolve([|dummy|])),
      getById: mockGetById |> getWithDefault((~id as _id) => resolve(dummy)),
      add: mockAdd |> getWithDefault((~paperClip as _add) => resolve(dummy)),
      update:
        mockUpdate |> getWithDefault((~id as _id, ~paperClip as _paperClip) => resolve(dummy)),
      remove: mockRemove |> getWithDefault((~id as _id) => resolve({"success": true}))
    };
};

module Handler = {
  let make =
      (~allPaperClips, ~paperClip, ~addPaperClip, ~updatePaperClip, ~removePaperClip)
      : PaperClipHandler.t('root) => {
    resolvers: PaperClipHandler.resolvers,
    queries: {
      "allPaperClips":
        allPaperClips |> getWithDefault((_root, _input, _context) => resolve([|dummy|])),
      "paperClip": paperClip |> getWithDefault((_root, _input, _context) => resolve(dummy))
    },
    mutations: {
      "addPaperClip": addPaperClip |> getWithDefault((_root, _input, _context) => resolve(dummy)),
      "updatePaperClip":
        updatePaperClip |> getWithDefault((_root, _input, _context) => resolve(dummy)),
      "removePaperClip":
        removePaperClip |> getWithDefault((_root, _input, _context) => resolve({"success": true}))
    }
  };
};
