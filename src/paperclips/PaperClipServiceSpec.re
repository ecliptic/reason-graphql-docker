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
            let size = PaperClip.Size.Small;
            resolve(fail("TODO"))
          }
        )
    )
);
