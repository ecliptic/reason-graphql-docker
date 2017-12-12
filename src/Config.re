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
  let url =
    getEnvVar(
      "DATABASE_URL",
      "postgres://reason_graphql_docker@localhost:5432/reason_graphql_docker"
    );
  let config = DbUtils.parseDbUrl(url);
  let name = config##database;
  let username = config##user;
  let password = config##password;
  let hostname = config##host;
  let port = config##port;
  let poolMin = parseInt(getEnvVar("DATABASE_POOL_MIN", "0"), 10);
  let poolMax = parseInt(getEnvVar("DATABASE_POOL_MAX", "10"), 10);
  let poolIdle = parseInt(getEnvVar("DATABASE_POOL_IDLE", "10000"), 10);
};
