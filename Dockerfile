FROM ocaml/opam:debian

USER root

ENV NODE_ENV=production

# Install OCaml toolchain
RUN opam update && \
  opam switch 4.02.3+buckle-master && \
  echo "eval `opam config env`" > /etc/profile.d/opam.sh && \
  rm -rf /home/opam/opam-repository

# Install Nodejs toolchain
RUN (curl -sL https://deb.nodesource.com/setup_8.x | bash -) && \
  apt-get install -y nodejs yarn --no-install-recommends && \
  apt-get clean && \
  rm -rf /var/cache/apt/archives/* /var/lib/apt/lists/*

RUN npm install -g bs-platform

# Create app directory
WORKDIR /usr/src/app

# Install app dependencies
COPY package.json ./
COPY yarn.lock ./
RUN yarn

# Bundle app source
COPY . .

CMD [ "yarn", "start" ]
