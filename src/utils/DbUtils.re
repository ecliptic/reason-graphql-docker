open Js.Promise;

open PromiseUtils;

let debug = ErrorUtils.Debug.make("DbUtils");

let debugExn = ErrorUtils.Debug.make("DbUtils:exn");

type host = {. "host": string, "port": Js.Nullable.t(string)};

type config = {
  .
  "driver": string,
  "user": Js.Nullable.t(string),
  "password": Js.Nullable.t(string),
  "host": Js.Nullable.t(string),
  "hosts": Js.Nullable.t(array(host)),
  "port": Js.Nullable.t(string),
  "database": Js.Nullable.t(string),
  "filename": Js.Nullable.t(string),
  "native": Js.Nullable.t(Js.boolean),
  "reconnect": Js.Nullable.t(Js.boolean)
};

[@bs.module] external parseDbUrl : string => config = "parse-database-url";

/* Database Exceptions */
/** Retrieve the "code" from a database exception */ [@bs.get]
external exnCode : Js.Promise.error => Js.Nullable.t(string) =
  "code";

/** Retrieve the "constraint" from a database exception */ [@bs.get]
external exnConstraint : Js.Promise.error => Js.Nullable.t(string) =
  "constraint";

/** Retrieve the "routine" from a database exception */ [@bs.get]
external exnRoutine : Js.Promise.error => Js.Nullable.t(string) =
  "routine";

let invalidTextRepresentation = "22P02";

let uniqueViolation = "23505";

/*
 * Pass-through error handlers - handles a specific error, letting all the rest flow through to the
 * next handler.
 */
/** Handles errors for the specified unique constraint */
let handleUniqueError = (~name: string, ~message: string, promise) =>
  promise
  |> catch(
       (exn) => {
         let continue = reject(toExn(exn));
         let codeOpt = exn |> exnCode |> Js.Nullable.to_opt;
         switch codeOpt {
         | Some(code) =>
           if (code === uniqueViolation) {
             let constraintOpt = exn |> exnConstraint |> Js.Nullable.to_opt;
             switch constraintOpt {
             | Some(constraintName) =>
               if (constraintName === name) {
                 reject(makeError(message))
               } else {
                 continue
               }
             | None => continue
             }
           } else {
             continue
           }
         | None => continue
         }
       }
     );

/*
 * Terminator error handler - handles all remaining unhandled DB errors
 */
/** Handle generic database errors */
let handleDbErrors = (promise) =>
  promise
  |> catch(
       (exn) => {
         let codeOpt = exn |> exnCode |> Js.Nullable.to_opt;
         switch codeOpt {
         | Some(code) =>
           if (code === uniqueViolation) {
             reject(makeError("A unique constraint was violated."))
           } else if (code === invalidTextRepresentation) {
             let routineOpt = exn |> exnRoutine |> Js.Nullable.to_opt;
             switch routineOpt {
             | Some(routine) =>
               if (routine === "string_to_uuid") {
                 reject(makeError("The database received an invalid format for a UUID"))
               } else {
                 reject(makeError("The database received an invalid text representation."))
               }
             | None => reject(makeError("The database received an invalid text representation."))
             }
           } else {
             debug("Unhandled error:");
             debugExn(exn);
             reject(makeError("A database error occurred."))
           }
         | None =>
           debug("Unhandled error:");
           debugExn(exn);
           reject(makeError("A database error occurred."))
         }
       }
     );
