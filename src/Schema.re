open GraphQLScalar;

[@bs.val] external __dirname : string = "";

[@bs.new] external createDate : unit => Js.Date.t = "Date";

let schema = Node.Fs.readFileAsUtf8Sync(__dirname ++ "/../graphql/schema.graphql");

let queries = Node.Fs.readFileAsUtf8Sync(__dirname ++ "/../graphql/queries.graphql");

type coreResolvers = {. "DateTime": scalarType(string, Js.Date.t)};

let coreResolvers: coreResolvers = {
  "DateTime":
    makeScalar({
      "name": "DateTime",
      "description": "DateTime custom scalar Type",
      "serialize": (value) => value |> Js.Date.getTime |> Js.Float.toString |> Js.Nullable.return,
      "parseValue": (str) => str |> Js.Date.fromString |> Js.Nullable.return,
      "parseLiteral": (ast) => {
        open Language;
        let kind = getAstKind(ast);
        if (kind === intKind) {
          ast |> getAstValue |> TextUtils.parseInt_(10) |> Js.Nullable.return
        } else {
          Js.Nullable.null
        }
      }
    })
};
