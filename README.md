# Reason-Express-Heroku

This is an Express-based application server written in [ReasonML](https://reasonml.github.io/).

## Installation

~~Easy deploy to Heroku~~ (currently blocked - see notes on deployment below)

### Dependencies

Install dependencies with [Yarn](http://yarnpkg.com):

    $ yarn

This should install the ReasonML and BuckleScript platform to compile the code to JavaScript.

### Database Setup

Create a local `.env` file (required by `dotenv-cli`):

    $ touch .env

Set up a local database:

    $ createuser reason_graphql_docker

    $ createdb reason_graphql_docker -O reason_graphql_docker

If you get an error about the "postgres" role not existing, you'll want to create it as a superuser:

    $ createuser -s postgres

Then, you can run the Knex migrations:

    $ yarn migrate

## Running the Application

To run in development, use a `bsb` watcher on a separate tab first:

    $ yarn build.watch

Then run the server in development mode:

    $ yarn dev

Inspect the GraphQL schema with the dev-only GraphiQL endpoint: http://localhost:4000/graphiql

## Deployment

Compiled intermediate code goes in the `lib/bs` and `lib/ocaml` folders, which are ignored by git. Resulting JavaScript is output in `lib/js`.

There is currently an issue running `bsb` in Docker and similar environments, with a [ticket](https://github.com/BuckleScript/bucklescript/issues/2336) open in the repository to investigate. In the meantime, the build needs to be run locally and pushed to your target container registry manually.

The drawback to this is that the application cannot easily be built remotely from the git tree alone. Features like continuous integration or Heroku review apps are more challenging to implement until the issue is resolved.

The `heroku.yml` file allows the container to easily be built and deployed on Heroku's container stack.
