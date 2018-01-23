[@bs.val] external parseInt_ : (string, int) => int = "parseInt";

/** parseInt with an assumed radix of 10 */
let parseInt = (string) => parseInt_(string, 10);

let getOrError = (~error: string, nullable: Js.Nullable.t('a)) =>
  switch (nullable |> Js.Nullable.to_opt) {
  | None => Js.Exn.raiseError(error)
  | Some(results) => results
  };

let decodeDate = MomentRe.((date: string) => moment(date) |> Moment.toDate);
