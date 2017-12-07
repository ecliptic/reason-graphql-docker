/**
 * A basic heartbeat route that always returns 200 "ok"
 */
let heartbeat = (_, res: Express.Response.t) => {
  Express.Response.sendString(res, "ok") |> ignore;
  Js.Promise.resolve(Js.undefined)
};
