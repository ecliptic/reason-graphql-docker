open BsAbstract;

open Js.Date;

module PaperClip = {
  [@bs.deriving jsConverter]
  type size = [ [@bs.as "SMALL"] | `Small [@bs.as "MEDIUM"] | `Medium [@bs.as "LARGE"] | `Large];
  type t = {
    id: string,
    createdAt: Js.Date.t,
    updatedAt: Js.Date.t,
    size
  };
};

module Json = {
  module PaperClip = {
    type t = {. "id": string, "createdAt": string, "updatedAt": string, "size": string};
    type input = {. "size": string};
  };
};

module Encode = {
  open Json.Encode;
  open Option.Infix;
  let paperClip = (paperClip: PaperClip.t) : Js.Json.t =>
    object_([
      ("id", paperClip.id |> string),
      ("createdAt", paperClip.createdAt <#> toISOString |> string),
      ("updatedAt", paperClip.updatedAt <#> toISOString |> string),
      ("size", paperClip.size |> string)
    ]);
};

module Decode = {
  open Json.Decode;
  let decode = (json: Js.Json.t) : PaperClip.t => {
    id: json |> field("id", string),
    createdAt: json |> field("created_at", string) |> Js.Date.fromString,
    updatedAt: json |> field("updated_at", string) |> Js.Date.fromString,
    size: json |> field("size", string) |> Size.fromString
  };
};
