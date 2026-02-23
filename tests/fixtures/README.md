# RASA Test Fixtures

This directory contains mock JSON files for testing RASA (RPKI AS-SET Authorization) functionality.

## Directory Structure

```
fixtures/
├── rasa-auth/          # RASA-AUTH test files
│   ├── valid/          # Valid RASA-AUTH configurations
│   ├── invalid/        # Invalid/malformed files
│   └── edge-cases/     # Edge case scenarios
├── rasa-set/           # RASA-SET test files
│   ├── valid/          # Valid RASA-SET configurations
│   ├── invalid/        # Invalid/malformed files
│   └── edge-cases/     # Edge case scenarios
└── bidirectional/      # Combined test scenarios
    ├── both-auth/      # Both RASA-AUTH and RASA-SET present
    ├── single-auth/    # Only one type present
    └── conflicts/      # Conflicting authorizations

## File Naming Convention

- `{test-case-id}-{description}.json`
- Examples:
  - `001-single-as.json`
  - `002-multiple-assets.json`
  - `003-empty-members.json`

## Test Coverage Goals

- RASA-AUTH: 40+ test scenarios
- RASA-SET: 40+ test scenarios
- Bidirectional: 30+ test scenarios
- Total: 110+ comprehensive tests
