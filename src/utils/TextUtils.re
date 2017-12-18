[@bs.val] external parseInt : (string, int) => int = "";

/* parseInt with the arguments flipped, for easy chaining */
let parseInt_ = (int, string) => parseInt(string, int);
