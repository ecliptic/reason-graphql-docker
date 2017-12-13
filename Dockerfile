FROM node:8-alpine

ENV NODE_ENV=production

ADD https://github.com/Yelp/dumb-init/releases/download/v1.1.1/dumb-init_1.1.1_amd64 /usr/local/bin/dumb-init

RUN addgroup -S app && \
  adduser -S -g app app && \
  \
  # dumb-init
  chmod +x /usr/local/bin/dumb-init && \
  \
  # Permissions
  mkdir -p /usr/src/app && \
  chown -R app:root /usr/src/app

USER app

WORKDIR /usr/src/app

COPY package.json .
COPY yarn.lock .
RUN yarn --prod
COPY . .

CMD [ "dumb-init", "yarn", "start" ]
