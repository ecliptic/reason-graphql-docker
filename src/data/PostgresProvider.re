type t = {knex: Knex.t};

/**
 * Initialize a new Postgres provider
 */
let make = () : Js.Promise.t(t) =>
  Js.Promise.make(
    (~resolve, ~reject) => {
      open Js.Nullable;
      open Js.Option;
      let connection =
        KnexConfig.Connection.make(
          ~user=Config.Database.username |> to_opt |> getWithDefault("paper_clips"),
          ~password=Config.Database.password |> to_opt |> getWithDefault("paper_clips"),
          ~host=Config.Database.hostname |> to_opt |> getWithDefault("localhost"),
          ~port=Config.Database.port |> to_opt |> getWithDefault("5432"),
          ~database=Config.Database.name |> to_opt |> getWithDefault("paper_clips"),
          ()
        );
      let pool =
        KnexConfig.Pool.make(
          ~min=Config.Database.poolMin,
          ~max=Config.Database.poolMax,
          ~idleTimeoutMillis=Config.Database.poolIdle,
          ()
        );
      let config =
        KnexConfig.make(~client="pg", ~connection, ~pool, ~acquireConnectionTimeout=2000, ());
      let knex = Knex.make(config);
      /* Verify the connection before proceeding */
      knex
      |> Knex.raw("select now()")
      /* Everything's good! Let's resolve with the interface */
      |> Js.Promise.then_(
           (result) => {
             [@bs] resolve({knex: knex});
             Js.Promise.resolve(result)
           }
         )
      /* Something went wrong */
      |> Js.Promise.catch(
           (_exn) => {
             let message = "Unable to connect to Knex. Ensure valid connection";
             Js.log(message);
             [@bs] reject(PromiseUtils.makeError(message));
             Js.Promise.reject(PromiseUtils.makeError(message))
           }
         )
      |> ignore
    }
  );
