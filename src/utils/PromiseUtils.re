open Js.Promise;

[@bs.new] external makeError : string => exn = "Error";

[@bs.get] external getMessage : Js.Promise.error => Js.Nullable.t(string) = "message";

external toJsExn : Js.Promise.error => Js.Exn.t = "%identity";

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
