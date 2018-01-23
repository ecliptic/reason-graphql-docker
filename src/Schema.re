module PaperClip = {
  [@bs.deriving jsConverter]
  type size = [ [@bs.as "SMALL"] | `Small [@bs.as "MEDIUM"] | `Medium [@bs.as "LARGE"] | `Large];
  type t = {
    .
    "id": Js.Nullable.t(string),
    "created_at": Js.Nullable.t(Js.Date.t),
    "updated_at": Js.Nullable.t(Js.Date.t),
    "size": string
  };
};
