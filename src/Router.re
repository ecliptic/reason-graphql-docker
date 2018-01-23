open Js.Obj;

module Web = {
  external asMiddleware : PromiseRouter.t => Express.Middleware.t = "%identity";
  let make = () => {
    let router = PromiseRouter.make();
    PromiseRouter.get(router, ~path="/", HttpUtils.heartbeat);
    asMiddleware(router)
  };
};

module GraphQL = {
  open GraphQLScalar;
  [@bs.val] external __dirname : string = "";
  [@bs.new] external createDate : unit => Js.Date.t = "Date";
  let path = [%raw {|require('path')|}];
  let schema =
    Node.Fs.readFileAsUtf8Sync([%raw {|path.join(__dirname, "../graphql/schema.graphql")|}]);
  let queries =
    Node.Fs.readFileAsUtf8Sync([%raw {|path.join(__dirname, "../graphql/queries.graphql")|}]);
  type coreResolvers = {. "DateTime": scalarType(string, Js.Date.t)};
  let coreResolvers: coreResolvers = {
    "DateTime":
      makeScalar({
        "name": "DateTime",
        "description": "DateTime custom scalar Type",
        "serialize": (value) => value |> Js.Date.toISOString |> Js.Nullable.return,
        "parseValue": (str) => str |> Js.Date.fromString |> Js.Nullable.return,
        "parseLiteral": (ast) => {
          open Language;
          let kind = getAstKind(ast);
          if (kind === intKind) {
            ast |> getAstValue |> Utils.parseInt |> Js.Nullable.return
          } else {
            Js.Nullable.null
          }
        }
      })
  };
  let makeSchema = (dataProvider: DataProvider.t) => {
    let paperClip = PaperClipHandler.make(dataProvider);
    let resolvers =
      empty()
      |> assign(coreResolvers)
      |> assign(paperClip.resolvers)
      |> assign({"Query": empty() |> assign(paperClip.queries)})
      |> assign({"Mutation": empty() |> assign(paperClip.mutations)});
    GraphQLTools.makeExecutableSchema({"typeDefs": schema ++ queries, "resolvers": resolvers})
  };
  let make = (dataProvider: DataProvider.t) =>
    makeSchema(dataProvider) |> ApolloServerExpress.createGraphQLExpressMiddleware;
};
