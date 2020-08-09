FROM golang:alpine as builder

RUN apk --no-cache add build-base git bzr mercurial gcc bash sqlite
WORKDIR /src

# download all dependencies
COPY go.mod /src
COPY go.sum /src
RUN go mod download

COPY . /src
RUN cd /src && go build -o server

FROM alpine
WORKDIR /app

COPY /dist /app/dist
COPY --from=builder /src/server /app/

EXPOSE 8000

CMD ./server


