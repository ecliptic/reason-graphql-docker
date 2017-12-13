[@bs.module "chalk"] external blue : string => string = "";

[@bs.module "chalk"] external green : string => string = "";

[@bs.module "chalk"] external red : string => string = "";

[@bs.val] external parseInt : (string, int) => int = "";

/* parseInt with the arguments flipped, for easy chaining */
let parseInt_ = (int, string) => parseInt(string, int);
