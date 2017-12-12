type query('row) = Js.Promise.t(array(Js.t('row)));

type t('options, 'row, 'a) = [@bs] ((string, 'options) => query('row));

[@bs.module] external make : 'connectionOpts => t('options, 'row, 'a) = "knex";

[@bs.send] external raw : (t('options, 'row, 'a), string) => Js.Promise.t('b) = "";

[@bs.send.pipe : query('row)] external where : 'a => query('row) = "";

[@bs.send.pipe : query('row)] external select : 'a => query('row) = "";

[@bs.send.pipe : query('row)] external insert : 'a => query('row) = "";

[@bs.send.pipe : query('row)] external update : 'a => query('row) = "";

[@bs.send.pipe : query('row)] external del : 'a => query('row) = "";

[@bs.send.pipe : query('row)] external returning : 'a => query('row) = "";

let decodeResults = (~decoder: Js.Json.t => 'a, results: array('row)) =>
  results |> Js.Array.map(HttpUtils.objToJson) |> Js.Array.map(decoder) |> Js.Promise.resolve;

let rejectIfEmpty = (~decoder, ~error, results) =>
  switch results {
  | [||] => Js.Promise.reject(PromiseUtils.makeError(error))
  | results => decodeResults(~decoder, results)
  };

let pickFirst = (results) => Js.Promise.resolve(results[0]);
