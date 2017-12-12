[@bs.val] external __dirname : string = "";

let schema = Node.Fs.readFileAsUtf8Sync(__dirname ++ "/../../graphql/schema.graphql");

let queries = Node.Fs.readFileAsUtf8Sync(__dirname ++ "/../../graphql/queries.graphql");
