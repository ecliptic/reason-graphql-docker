open Js.Obj;

module Web = {
  external asMiddleware : PromiseUtils.Router.t => Express.Middleware.t = "%identity";
  let make = () => {
    let router = PromiseUtils.Router.make();
    PromiseUtils.Router.get(router, ~path="/", HttpUtils.heartbeat);
    asMiddleware(router)
  };
};

module GraphQL = {
  let makeSchema = (dataProvider: DataProvider.t('queryRow)) => {
    let paperClip = PaperClipHandler.make(dataProvider);
    let resolvers =
      empty()
      |> assign(paperClip.resolvers)
      |> assign({"Query": empty() |> assign(paperClip.queries)})
      |> assign({"Mutation": empty() |> assign(paperClip.mutations)});
    GraphQLTools.makeExecutableSchema({
      "typeDefs": Schema.schema ++ Schema.queries,
      "resolvers": resolvers
    })
  };
  let make = (dataProvider: DataProvider.t('queryRow)) =>
    makeSchema(dataProvider) |> ApolloServerExpress.createGraphQLExpressMiddleware;
};
