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
            open Schema.PaperClip;
            let size = `Small;
            resolve(fail("TODO"))
          }
        )
    )
);
