open Chalk;

open Express.App;

open Js.Promise;

[@bs.module] external cors : 'a => Express.Middleware.t = "";

[@bs.module] external morgan : string => Express.Middleware.t = "";

[@bs.module "body-parser"] external json : 'a => Express.Middleware.t = "";

let debug = Debug.make("paperclip-api", "Server");

let debugExn = Debug.make("paperclip-api", "Server:exn");

let graphiqlMiddleware = ApolloServerExpress.createGraphiQLExpressMiddleware("/graphql");

/**
 * Report listen status
 */
let onListen = (exn) =>
  switch (Js.Nullable.to_opt(exn)) {
  | Some(err) =>
    debug("Express listen error");
    debugExn(err)
  | None =>
    Js.log(
      blue("trailmap-api")
      ++ " is listening on port "
      ++ green(Js.Int.toString(Config.Server.port))
    )
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
         debug("Uncaught server error");
         debugExn(error |> PromiseUtils.toJsExn);
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
