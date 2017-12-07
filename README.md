# Reason-Express-Heroku

This is an Express-based application server written in [ReasonML](https://reasonml.github.io/).

## Installation

### Dependencies

Install dependencies with [Yarn](http://yarnpkg.com):

    $ yarn

This should install the ReasonML and BuckleScript platform to compile the code to JavaScript.

## Running the Application

To run in development, use a `bsb` watcher on a separate tab first:

    $ yarn build.watch

Then run the server in development mode:

    $ yarn dev

## Deployment

Compiled intermediate code goes in the `lib` folder, which is ignored by git. Resulting JavaScript is output inline in the source tree. The `yarn build` command is called by the Dockerfile to build the application.
