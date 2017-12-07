module Web = {
  external asMiddleware : PromiseUtils.Router.t => Express.Middleware.t = "%identity";
  let make = () => {
    let router = PromiseUtils.Router.make();
    PromiseUtils.Router.get(router, ~path="/", HttpUtils.heartbeat);
    asMiddleware(router)
  };
};
