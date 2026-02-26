# RASA Test Fixtures

This directory contains JSON test fixtures for RASA (RPKI AS-SET Authorization) functionality.

**IMPORTANT**: The full set of 6,787 fixtures is NOT stored in git. They are generated at build time.

## Directory Structure

```
fixtures/
├── sample/                    # Representative examples (stored in git)
│   ├── fallback_irrFallback_none_1.json
│   ├── hierarchy_2level_irrFallback_1.json
│   ├── edge_hurricane_electric_pattern.json
│   ├── edge_google_rasaonly_pattern.json
│   └── boundary_max_16bit_public_irrFallback_4.json
├── *.json                     # Generated fixtures (NOT in git)
└── README.md                  # This file
```

## Generating Fixtures

### Full Set (6,787 fixtures)

```bash
make fixtures
```

### Quick Test (Sample fixtures only)

```bash
make check-sample
```

### Full Test Suite

```bash
make check-full
```

## Generator Scripts

Located in `tests/generator/`:

- **schemas/rasa_schema.py** - Type definitions
- **combination_engine.py** - Generates flag combinations
- **fixture_generator.py** - Produces JSON fixtures
- **c_test_generator.py** - Generates C test code

## Fixture Categories

| Category | Count | Description |
|----------|-------|-------------|
| fallback_ | 32 | Fallback mode × flag combinations |
| hierarchy_ | 264 | 1-5 level hierarchies |
| asn_ | 5,208 | ASN boundaries (16/32-bit) |
| irr_source_ | 96 | 12 IRR databases |
| rasa_auth_ | 1,000 | Authorization scenarios |
| combined_ | 144 | Integration tests |
| boundary_ | 35 | Edge cases |
| edge_ | 8 | Real-world patterns |

## Why Build-Time Generation?

1. **Repository Size**: 6,787 files = ~280,000 lines
2. **Single Source**: Python scripts are the truth
3. **CI/CD**: Fresh fixtures catch generator bugs
4. **Deterministic**: Same scripts always produce same output

The 5 sample fixtures demonstrate the format and enable quick testing.
