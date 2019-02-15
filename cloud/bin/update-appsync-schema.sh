#!/usr/bin/env bash -e

source ./.env
aws appsync get-introspection-schema --api-id $APP_SYNC_API_ID --format JSON ../client/graphql_schema.json