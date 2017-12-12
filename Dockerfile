FROM node:8-alpine

WORKDIR /

ADD https://github.com/Yelp/dumb-init/releases/download/v1.1.1/dumb-init_1.1.1_amd64 /usr/local/bin/dumb-init

RUN addgroup -S app \
  && adduser -S -g app app \
  \
  # make dumb-init executable
  && chmod +x /usr/local/bin/dumb-init \
  \
  # Permissions
  && mkdir -p /usr/src/app \
  && chown -R app:root /usr/src/app \
  && chmod -R 0770 /usr/src/app \
  \
  # Dependencies for building ninja-build
  && apk add --update python g++ make;

USER app

WORKDIR /usr/src/app

ENV NODE_ENV=production

COPY package.json .
COPY yarn.lock .
RUN yarn --prod
COPY . .
RUN yarn build

CMD [ "dumb-init", "yarn", "start" ]
