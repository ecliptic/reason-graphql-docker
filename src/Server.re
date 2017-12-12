open Express.App;

open Js.Promise;

[@bs.module] external cors : 'a => Express.Middleware.t = "";

[@bs.module] external morgan : string => Express.Middleware.t = "";

[@bs.module "body-parser"] external json : 'a => Express.Middleware.t = "";

let graphiqlMiddleware = ApolloServerExpress.createGraphiQLExpressMiddleware("/graphql");

/**
 * Report listen status
 */
let onListen = (exn) => {
  let error = Js.Nullable.to_opt(exn);
  switch error {
  | Some(err) =>
    Js.log(
      TextUtils.red("Express listen error: ")
      ++ Js.Option.getWithDefault("(no message)", Js.Exn.message(err))
    )
  | None =>
    Js.log(
      TextUtils.blue("telepathic")
      ++ " is listening on port "
      ++ TextUtils.green(Js.Int.toString(Config.Server.port))
    )
  }
};

/**
 * The start routine for the application server
 */
let start = (~graphRouter) => {
  let app = Express.App.make();
  /* Determine the log style based on the environment */
  Config.Server.isDev ? use(app, morgan("dev")) : use(app, morgan("combined"));
  /*
   * General Express middleware
   */
  use(app, cors({"exposedHeaders": Config.Server.corsHeaders}));
  use(app, json({"limit": Config.Server.bodyLimit}));
  useOnPath(app, graphRouter, ~path="/graphql");
  if (Config.Server.isDev) {
    useOnPath(app, graphiqlMiddleware, ~path="/graphiql")
  };
  /*
   * Request routing
   */
  use(app, Router.Web.make());
  /*
   * Start taking requests
   */
  listen(app, ~port=Config.Server.port, ~onListen, ())
};

/**
 * The main application init routine
 */
let main = () =>
  DataProvider.make()
  |> then_(
       (dataProvider) => {
         let graphRouter = Router.GraphQL.make(dataProvider);
         start(~graphRouter);
         resolve()
       }
     )
  |> catch(
       (error) => {
         Js.log(
           TextUtils.red(
             Js.Option.getWithDefault(
               "(no message)",
               Js.Nullable.to_opt(ErrorUtils.getMessage(error))
             )
           )
         );
         Node.Process.exit(1);
         Js.Promise.resolve()
       }
     );

/**
 * If this module was run from the command line, execute the main() routine
 */
if ([%bs.raw "require.main === module"]) {
  main() |> ignore
};
