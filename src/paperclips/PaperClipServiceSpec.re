open Js.Promise;

open Jest;

describe(
  "PaperClipService",
  () =>
    describe(
      "getAll(~size)",
      () =>
        Skip.testPromise(
          "",
          () => {
            let size = Schema.PaperClip.Size.Small;
            resolve(fail("TODO"))
          }
        )
    )
);
