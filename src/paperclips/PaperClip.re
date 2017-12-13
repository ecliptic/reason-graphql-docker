module Size = {
  type t =
    | Small
    | Medium
    | Large;
  let fromString = (str: string) =>
    switch str {
    | "SMALL" => Small
    | "MEDIUM" => Medium
    | "LARGE" => Large
    /* Default to Small, which shouldn't happen if GraphQL schema validation is working */
    | _ => Small
    };
  let toString = (size: t) =>
    switch size {
    | Small => "SMALL"
    | Medium => "MEDIUM"
    | Large => "LARGE"
    };
};

type t = {
  id: string,
  createdAt: Js.Date.t,
  updatedAt: Js.Date.t,
  size: Size.t
};

module Decode = {
  let paperClip = (json) =>
    Json.Decode.{
      id: json |> field("id", string),
      createdAt: json |> field("created_at", string) |> Js.Date.fromString,
      updatedAt: json |> field("updated_at", string) |> Js.Date.fromString,
      size: json |> field("size", string) |> Size.fromString
    };
};

module Resolve = {
  let id = (paperClip: t) => paperClip.id;
  let createdAt = (paperClip: t) => paperClip.createdAt;
  let updatedAt = (paperClip: t) => paperClip.updatedAt;
  let size = (paperClip: t) => paperClip.size |> Size.toString;
};

module Encode = {
  open Json.Encode;
  open Resolve;
  let paperClip = (paperClip: t) =>
    object_([
      ("id", paperClip |> id |> string),
      ("createdAt", paperClip |> createdAt |> Js.Date.toISOString |> string),
      ("updatedAt", paperClip |> updatedAt |> Js.Date.toISOString |> string),
      ("size", paperClip |> size |> string)
    ]);
};

type paperClipJson = {. "id": string, "createdAt": string, "updatedAt": string, "size": string};

type paperClipInput = {. "size": string};

let resolvers = {
  "PaperClip": {
    "id": Resolve.id,
    "createdAt": Resolve.createdAt,
    "updatedAt": Resolve.updatedAt,
    "size": Resolve.size
  }
};
