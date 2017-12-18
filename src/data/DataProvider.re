open Js.Promise;

type t = {postgres: PostgresProvider.t};

let make = () : Js.Promise.t(t) => {
  let timeout = PromiseUtils.cancelTimeout(10000, "DataProvider failed to initialize.");
  let done_ = PostgresProvider.make();
  race([|timeout, done_|]) |> then_((postgres) => resolve({postgres: postgres}))
};
