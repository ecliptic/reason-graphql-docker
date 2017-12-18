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
  let makeSchema = (dataProvider: DataProvider.t) => {
    let paperClip = PaperClipHandler.make(dataProvider);
    let resolvers =
      empty()
      |> assign(Schema.coreResolvers)
      |> assign(paperClip.resolvers)
      |> assign({"Query": empty() |> assign(paperClip.queries)})
      |> assign({"Mutation": empty() |> assign(paperClip.mutations)});
    GraphQLTools.makeExecutableSchema({
      "typeDefs": Schema.schema ++ Schema.queries,
      "resolvers": resolvers
    })
  };
  let make = (dataProvider: DataProvider.t) =>
    makeSchema(dataProvider) |> ApolloServerExpress.createGraphQLExpressMiddleware;
};
