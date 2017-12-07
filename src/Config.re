[@bs.module "dotenv"] external config : {. "silent": bool} => unit = "config";

[@bs.val] external parseInt : (string, int) => int = "";

/* Load environment variables from a .env file if it exists */
config({"silent": true});

let getEnvVar = (key, fallback) =>
  Js.Option.getWithDefault(fallback, Js.Dict.get(Node.Process.process##env, key));

module Server = {
  let port = parseInt(getEnvVar("PORT", "4000"), 10);
  let bodyLimit = "100kb";
  let corsHeaders = [|"Link"|];
  let isDev = getEnvVar("NODE_ENV", "development") === "development";
};

module Database = {
  let url = getEnvVar("DATABASE_URL", "postgres://username:password@localhost:5432/database");
};
