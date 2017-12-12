open Js.Promise;

type t('queryRow) = {postgres: PostgresProvider.t('queryRow)};

let make = () : Js.Promise.t(t('queryRow)) => {
  let timeout = PromiseUtils.cancelTimeout(10000, "DataProvider failed to initialize.");
  let done_ = PostgresProvider.make();
  race([|timeout, done_|]) |> then_((postgres) => resolve({postgres: postgres}))
};
