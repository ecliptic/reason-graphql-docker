type t('queryRow) = {fromTable: (~name: string) => Knex.query('queryRow)};

/**
 * Initialize a new Postgres provider
 */
let make = () : Js.Promise.t(t('queryRow)) =>
  Js.Promise.make(
    (~resolve, ~reject) => {
      let knex =
        Knex.make({
          "client": "pg",
          "connection": {
            "user": Config.Database.username,
            "password": Config.Database.password,
            "host": Config.Database.hostname,
            "port": Config.Database.port,
            "database": Config.Database.name
          },
          "pool": {
            "min": Config.Database.poolMin,
            "max": Config.Database.poolMax,
            "idleTimeoutMillis": Config.Database.poolIdle
          },
          "acquireConnectionTimeout": 2000
        });
      /* Define the DataProvider interface */
      let interface = {fromTable: (~name) => [@bs] knex(name, "")};
      /* Verify the connection before proceeding */
      Knex.raw(knex, "select now()")
      /* Everything's good! Let's resolve with the interface */
      |> Js.Promise.then_(
           (result) => {
             [@bs] resolve(interface);
             Js.Promise.resolve(result)
           }
         )
      /* Something went wrong */
      |> Js.Promise.catch(
           (_exn) => {
             let message = "Unable to connect to knex.  Ensure valid connection";
             Js.log(message);
             [@bs] reject(PromiseUtils.makeError(message));
             Js.Promise.reject(PromiseUtils.makeError(message))
           }
         )
      |> ignore
    }
  );
