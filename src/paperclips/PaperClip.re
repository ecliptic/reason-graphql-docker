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

type paperClipInput = {. "size": string};

type paperClipJson = {. "id": string, "createdAt": string, "updatedAt": string, "size": string};

module Resolve = {
  let id = (paperClip: t) => paperClip.id;
  let createdAt = (paperClip: t) => paperClip.createdAt |> Js.Date.toISOString;
  let updatedAt = (paperClip: t) => paperClip.updatedAt |> Js.Date.toISOString;
  let size = (paperClip: t) => paperClip.size |> Size.toString;
};

let encode = (paperClip: t) : Js.Json.t =>
  Json.Encode.(
    Resolve.(
      object_([
        ("id", paperClip |> id |> string),
        ("createdAt", paperClip |> createdAt |> string),
        ("updatedAt", paperClip |> updatedAt |> string),
        ("size", paperClip |> size |> string)
      ])
    )
  );

let decode = (json: Js.Json.t) : t =>
  Json.Decode.{
    id: json |> field("id", string),
    createdAt: json |> field("created_at", string) |> Js.Date.fromString,
    updatedAt: json |> field("updated_at", string) |> Js.Date.fromString,
    size: json |> field("size", string) |> Size.fromString
  };
