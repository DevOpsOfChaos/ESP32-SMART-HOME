# Contributing

Keep changes small, explicit, and easy to verify.

## Rules

- do not commit secrets, tokens, passwords, or local environment files
- do not add private school material or internal working artifacts
- prefer changes that improve the public release story
- keep documentation and implementation aligned
- avoid broad refactors unless there is a clear technical reason

## What good contributions look like

- a focused fix with a clear reason
- a documentation update that matches the code or hardware
- a structural cleanup that makes the repo easier to use
- a test or validation step that proves the change did not break the contract

## Special care required

Treat these areas as core interfaces:

- device communication
- the master/server boundary
- device state and payload contracts
- public documentation and onboarding

## Working style

- make the result understandable to a new builder
- prefer fewer, safer steps over big uncontrolled rearrangements
- if a change affects a public interface, document it
