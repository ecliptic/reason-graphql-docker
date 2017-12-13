module Language = {
  include GraphQL.Language;
  [@bs.module "graphql/language"] external kinds : Js.Dict.t(string) = "Kind";
  [@bs.get] external getAstKind : astNode => string = "kind";
  [@bs.get] external getAstValue : astNode => string = "value";
  let intKind = Js.Dict.get(kinds, "INT") |> Js.Option.getWithDefault("IntValue");
  let floatKind = Js.Dict.get(kinds, "FLOAT") |> Js.Option.getWithDefault("FloatValue");
  let stringKind = Js.Dict.get(kinds, "STRING") |> Js.Option.getWithDefault("StringValue");
  let booleanKind = Js.Dict.get(kinds, "BOOLEAN") |> Js.Option.getWithDefault("BooleanValue");
  let nullKind = Js.Dict.get(kinds, "NULL") |> Js.Option.getWithDefault("NullValue");
  let enumKind = Js.Dict.get(kinds, "ENUM") |> Js.Option.getWithDefault("EnumValue");
  let listKind = Js.Dict.get(kinds, "LIST") |> Js.Option.getWithDefault("ListValue");
  let objectKind = Js.Dict.get(kinds, "OBJECT") |> Js.Option.getWithDefault("ObjectValue");
  let objectFieldKind =
    Js.Dict.get(kinds, "OBJECT_FIELD") |> Js.Option.getWithDefault("ObjectField");
};

type scalarType('serialized, 'parsed);

type scalarTypeInput('serialized, 'parsed) = {
  .
  "name": string,
  "description": string,
  "serialize": 'parsed => Js.Nullable.t('serialized),
  "parseValue": 'serialized => Js.Nullable.t('parsed),
  "parseLiteral": Language.astNode => Js.Nullable.t(int)
};

[@bs.new] [@bs.module "graphql"]
external makeScalar : scalarTypeInput('serialized, 'parsed) => scalarType('serialized, 'parsed) =
  "GraphQLScalarType";
