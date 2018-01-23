open Js.Option;

open Js.Promise;

[@bs.new] external makeError : string => exn = "Error";

[@bs.get] external getMessage : Js.Promise.error => Js.Nullable.t(string) = "message";

external toJsExn : Js.Promise.error => Js.Exn.t = "%identity";

let rejectWith = (message) => reject(makeError(message));

let logMessage = (exn: error) => {
  let message = Js.Exn.message(toJsExn(exn));
  Js.log(
    switch message {
    | Some(msg) => msg
    | None => "(no error message)"
    }
  )
};

let cancelTimeout = (delay: int, message: string) =>
  make(
    (~resolve as _, ~reject) =>
      Js.Global.setTimeout(() => [@bs] reject(makeError(message)), delay) |> ignore
  );

let toString = (obj) => obj |> Js.Json.stringifyAny |> Js.Option.getExn;

let getOrReject = (opt) =>
  switch opt {
  | Some(value) => resolve(value)
  | None => rejectWith("Unable to get")
  };

let expectToReject = (~withError) =>
  catch(
    (exn) =>
      exn
      |> getMessage
      |> Js.Nullable.to_opt
      |> getWithDefault("Invalid message")
      |> (
        (error) =>
          error === withError ?
            resolve(Jest.pass) :
            resolve(
              Jest.fail(
                "Error messages did not match\nExpected: "
                ++ withError
                ++ "\n"
                ++ "Received: "
                ++ error
              )
            )
      )
  );

/**
   * Reject a promise with the given error if the results are an empty array
   */
let rejectIfEmpty = (~error, results) =>
  switch results {
  | [||] => reject(makeError(error))
  | results => resolve(results)
  };

let pickFirst = (results) => resolve(results[0]);
