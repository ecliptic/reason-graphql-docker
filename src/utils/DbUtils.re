type host = {. "host": string, "port": Js.Nullable.t(string)};

type config = {
  .
  "driver": string,
  "user": Js.Nullable.t(string),
  "password": Js.Nullable.t(string),
  "host": Js.Nullable.t(string),
  "port": Js.Nullable.t(string),
  "database": Js.Nullable.t(string),
  "filename": Js.Nullable.t(string),
  "native": Js.Nullable.t(Js.boolean),
  "reconnect": Js.Nullable.t(Js.boolean)
};

[@bs.module] external parseDbUrl : string => config = "parse-database-url";
