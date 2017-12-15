module Debug = {
  [@bs.module] external debug : string => [@bs] ('a => unit) = "";
  let prefix = "trailmap-api";
  let make = (label: string) => {
    let log = debug(prefix ++ ":" ++ label);
    (message) => [@bs] log(message)
  };
};
