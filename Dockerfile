FROM node:alpine

# Default environment variables
ENV PORT=4000 \
  NODE_ENV=production

# Create app directory
RUN mkdir -p /srv/reason-graphql-docker
WORKDIR /srv/reason-graphql-docker

# Copy app binaries
COPY ./Server.bs /srv/reason-graphql-docker

EXPOSE $PORT
CMD ["/srv/reason-graphql-docker/Server.bs"]
