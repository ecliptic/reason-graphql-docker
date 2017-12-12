/**
 * A basic heartbeat route that always returns 200 "ok"
 */
let heartbeat = (_, res: Express.Response.t) => {
  Express.Response.sendString(res, "ok") |> ignore;
  Js.Promise.resolve(Js.undefined)
};

/**
 * Safely converts an object to json by stringifying it and parsing the results.
 */
let objToJson = (obj: Js.t('a)) : Js.Json.t => {
  let str = Js.Json.stringifyAny(obj);
  /* parseExn should be safe because the string came from stringifyAny */
  switch str {
  | Some(str) => str |> Js.Json.parseExn
  | None => Js.Json.null
  }
};
