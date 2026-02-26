#!/usr/bin/env python3
import json
import os
from typing import Dict, List, Any, Optional
from schemas.rasa_schema import (
    FallbackMode, RasaSetFlag, RasaAuthFlag, PropagationScope,
    RasaSetConfig, RasaAuthConfig, AuthorizedEntry
)
from combination_engine import CombinationEngine


class FixtureGenerator:
    def __init__(self, output_dir: str = "../fixtures"):
        self.output_dir = output_dir
        self.engine = CombinationEngine()
        os.makedirs(output_dir, exist_ok=True)
    
    def generate_all_fixtures(self) -> Dict[str, int]:
        counts = {
            'fallback_mode': 0,
            'hierarchy': 0,
            'asn_range': 0,
            'irr_source': 0,
            'rasa_auth': 0,
            'combined': 0,
            'boundary': 0,
            'edge_cases': 0,
            'total': 0
        }
        
        counts['fallback_mode'] = self._generate_fallback_mode_fixtures()
        counts['hierarchy'] = self._generate_hierarchy_fixtures()
        counts['asn_range'] = self._generate_asn_range_fixtures()
        counts['irr_source'] = self._generate_irr_source_fixtures()
        counts['rasa_auth'] = self._generate_rasa_auth_fixtures()
        counts['combined'] = self._generate_combined_fixtures()
        counts['boundary'] = self._generate_boundary_fixtures()
        counts['edge_cases'] = self._generate_edge_case_fixtures()
        counts['total'] = sum(counts.values())
        
        return counts
    
    def _generate_fallback_mode_fixtures(self) -> int:
        count = 0
        
        for test in self.engine.generate_all_fallback_mode_tests():
            fixture = {
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': test['config'].as_set_name,
                            'containing_as': test['config'].containing_as,
                            'members': test['config'].members,
                            'nested_sets': test['config'].nested_sets,
                            'fallback_mode': test['config'].fallback_mode.value,
                            'irr_source': test['config'].irr_source,
                            'flags': test['config'].flags.value.split('+') if '+' in test['config'].flags.value else [test['config'].flags.value] if test['config'].flags.value != 'none' else []
                        }
                    }
                ],
                'rasas': [],
                'expected_behavior': test['expected'],
                'test_metadata': {
                    'name': test['name'],
                    'type': test['type'],
                    'should_succeed': test['should_succeed']
                }
            }
            
            filename = f"{test['name']}.json"
            self._write_fixture(filename, fixture)
            count += 1
        
        return count
    
    def _generate_hierarchy_fixtures(self) -> int:
        count = 0
        
        for levels in [1, 2, 3, 4, 5]:
            for fallback_mode in FallbackMode:
                for test in self.engine.generate_hierarchy_tests(levels, fallback_mode):
                    rasa_sets = []
                    for cfg in test['hierarchy']:
                        rasa_sets.append({
                            'rasa_set': {
                                'as_set_name': cfg.as_set_name,
                                'containing_as': cfg.containing_as,
                                'members': cfg.members,
                                'nested_sets': cfg.nested_sets,
                                'fallback_mode': cfg.fallback_mode.value,
                                'irr_source': cfg.irr_source,
                                'flags': cfg.flags.value.split('+') if '+' in cfg.flags.value else [cfg.flags.value] if cfg.flags.value != 'none' else []
                            }
                        })
                    
                    fixture = {
                        'rasa_sets': rasa_sets,
                        'rasas': [],
                        'expected_expansion': test['expected_expansion'],
                        'test_metadata': {
                            'name': test['name'],
                            'type': test['type'],
                            'levels': test['levels'],
                            'fallback_mode': test['fallback_mode'].value
                        }
                    }
                    
                    filename = f"{test['name']}.json"
                    self._write_fixture(filename, fixture)
                    count += 1
        
        return count
    
    def _generate_asn_range_fixtures(self) -> int:
        count = 0
        
        for test in self.engine.generate_asn_range_tests():
            fixture = {
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': test['config'].as_set_name,
                            'containing_as': test['config'].containing_as,
                            'members': test['config'].members,
                            'nested_sets': test['config'].nested_sets,
                            'fallback_mode': test['config'].fallback_mode.value,
                            'irr_source': test['config'].irr_source,
                            'flags': test['config'].flags.value.split('+') if '+' in test['config'].flags.value else [test['config'].flags.value] if test['config'].flags.value != 'none' else []
                        }
                    }
                ],
                'rasas': [],
                'expected_behavior': test['expected'],
                'test_metadata': {
                    'name': test['name'],
                    'type': test['type'],
                    'asn_range': test.get('asn_range'),
                    'should_succeed': test['should_succeed']
                }
            }
            
            filename = f"{test['name']}.json"
            self._write_fixture(filename, fixture)
            count += 1
        
        return count
    
    def _generate_irr_source_fixtures(self) -> int:
        count = 0
        
        for test in self.engine.generate_irr_source_tests():
            fixture = {
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': test['config'].as_set_name,
                            'containing_as': test['config'].containing_as,
                            'members': test['config'].members,
                            'nested_sets': test['config'].nested_sets,
                            'fallback_mode': test['config'].fallback_mode.value,
                            'irr_source': test['config'].irr_source,
                            'flags': test['config'].flags.value.split('+') if '+' in test['config'].flags.value else [test['config'].flags.value] if test['config'].flags.value != 'none' else []
                        }
                    }
                ],
                'rasas': [],
                'expected_behavior': test['expected'],
                'test_metadata': {
                    'name': test['name'],
                    'type': test['type'],
                    'irr_source': test.get('irr_source'),
                    'should_succeed': test['should_succeed']
                }
            }
            
            filename = f"{test['name']}.json"
            self._write_fixture(filename, fixture)
            count += 1
        
        return count
    
    def _generate_rasa_auth_fixtures(self) -> int:
        count = 0
        
        for test in self.engine.generate_rasa_auth_tests():
            auth_cfg = test['auth_config']
            rasas = []
            for entry in auth_cfg.authorized_in:
                rasas.append({
                    'rasa': {
                        'authorized_as': auth_cfg.authorized_as,
                        'authorized_in': [
                            {
                                'entry': {
                                    'asset': entry.as_set_name
                                },
                                'propagation': entry.propagation.value
                            }
                        ],
                        'flags': [auth_cfg.flags.value] if auth_cfg.flags.value != 'none' else []
                    }
                })
            
            fixture = {
                'rasa_sets': [],
                'rasas': rasas,
                'expected_authorization': test['expected_authorized'],
                'test_metadata': {
                    'name': test['name'],
                    'type': test['type'],
                    'auth_flag': test.get('auth_flag'),
                    'propagation': test.get('propagation'),
                    'should_succeed': test['should_succeed']
                }
            }
            
            filename = f"{test['name']}.json"
            self._write_fixture(filename, fixture)
            count += 1
        
        return count
    
    def _generate_combined_fixtures(self) -> int:
        count = 0
        
        for test in self.engine.generate_combined_tests():
            set_cfg = test['set_config']
            auth_cfg = test['auth_config']
            
            rasa_sets = [{
                'rasa_set': {
                    'as_set_name': set_cfg.as_set_name,
                    'containing_as': set_cfg.containing_as,
                    'members': set_cfg.members,
                    'nested_sets': set_cfg.nested_sets,
                    'fallback_mode': set_cfg.fallback_mode.value,
                    'irr_source': set_cfg.irr_source,
                    'flags': set_cfg.flags.value.split('+') if '+' in set_cfg.flags.value else [set_cfg.flags.value] if set_cfg.flags.value != 'none' else []
                }
            }]
            
            rasas = []
            for entry in auth_cfg.authorized_in:
                rasas.append({
                    'rasa': {
                        'authorized_as': auth_cfg.authorized_as,
                        'authorized_in': [
                            {
                                'entry': {
                                    'asset': entry.as_set_name
                                },
                                'propagation': entry.propagation.value
                            }
                        ],
                        'flags': [auth_cfg.flags.value] if auth_cfg.flags.value != 'none' else []
                    }
                })
            
            fixture = {
                'rasa_sets': rasa_sets,
                'rasas': rasas,
                'expected_behavior': {
                    'set_fallback': set_cfg.fallback_mode.value,
                    'auth_flag': auth_cfg.flags.value,
                    'authorized': True
                },
                'test_metadata': {
                    'name': test['name'],
                    'type': test['type'],
                    'should_succeed': test['should_succeed']
                }
            }
            
            filename = f"{test['name']}.json"
            self._write_fixture(filename, fixture)
            count += 1
        
        return count
    
    def _generate_boundary_fixtures(self) -> int:
        count = 0
        
        for test in self.engine.generate_boundary_tests():
            if test['type'] == 'invalid':
                fixture = {
                    'rasa_sets': [{
                        'rasa_set': {
                            'as_set_name': f"AS-{test['config']['name'].upper()}",
                            'containing_as': test['config'].get('containing_as', 64496),
                            'members': test['config'].get('members', []),
                            'nested_sets': [],
                            'fallback_mode': test['config']['fallback_mode'].value,
                            'irr_source': test['config'].get('irr_source'),
                            'flags': []
                        }
                    }],
                    'rasas': [],
                    'expected_error': test['expected_error'],
                    'test_metadata': {
                        'name': test['name'],
                        'type': test['type'],
                        'should_succeed': test['should_succeed']
                    }
                }
            else:
                fixture = {
                    'rasa_sets': [
                        {
                            'rasa_set': {
                                'as_set_name': test['config'].as_set_name,
                                'containing_as': test['config'].containing_as,
                                'members': test['config'].members,
                                'nested_sets': test['config'].nested_sets,
                                'fallback_mode': test['config'].fallback_mode.value,
                                'irr_source': test['config'].irr_source,
                                'flags': test['config'].flags.value.split('+') if '+' in test['config'].flags.value else [test['config'].flags.value] if test['config'].flags.value != 'none' else []
                            }
                        }
                    ],
                    'rasas': [],
                    'expected_behavior': test['expected'],
                    'test_metadata': {
                        'name': test['name'],
                        'type': test['type'],
                        'boundary_type': test.get('boundary_type'),
                        'asn': test.get('asn'),
                        'should_succeed': test['should_succeed']
                    }
                }
            
            filename = f"{test['name']}.json"
            self._write_fixture(filename, fixture)
            count += 1
        
        return count
    
    def _generate_edge_case_fixtures(self) -> int:
        count = 0
        
        edge_cases = [
            {
                'name': 'irrlock_missing_source',
                'description': 'irrLock mode without irr_source should fail',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-INVALID-LOCK',
                            'containing_as': 64496,
                            'members': [],
                            'nested_sets': [],
                            'fallback_mode': 'irrLock',
                            'irr_source': None,
                            'flags': []
                        }
                    }
                ],
                'expected_error': 'irrLock requires irr_source'
            },
            {
                'name': 'rasaonly_empty_members',
                'description': 'rasaOnly mode with empty members should fail',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-INVALID-ONLY',
                            'containing_as': 64496,
                            'members': [],
                            'nested_sets': [],
                            'fallback_mode': 'rasaOnly',
                            'irr_source': None,
                            'flags': []
                        }
                    }
                ],
                'expected_error': 'rasaOnly requires members'
            },
            {
                'name': 'circular_reference_2level',
                'description': 'AS-A includes AS-B, AS-B includes AS-A',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-A',
                            'containing_as': 64496,
                            'members': [64497],
                            'nested_sets': ['AS-B'],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    },
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-B',
                            'containing_as': 64497,
                            'members': [64498],
                            'nested_sets': ['AS-A'],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    }
                ],
                'expected_warning': 'Circular reference detected'
            },
            {
                'name': 'hurricane_electric_pattern',
                'description': 'Real-world Hurricane Electric irrLock pattern',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-HURRICANE',
                            'containing_as': 6939,
                            'members': [],
                            'nested_sets': [],
                            'fallback_mode': 'irrLock',
                            'irr_source': 'RADB',
                            'flags': []
                        }
                    }
                ],
                'expected_behavior': {
                    'should_query_irr': True,
                    'irr_source': 'RADB'
                }
            },
            {
                'name': 'google_rasaonly_pattern',
                'description': 'Real-world Google rasaOnly pattern',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-GOOGLE',
                            'containing_as': 15169,
                            'members': [15169, 16591, 19425],
                            'nested_sets': [],
                            'fallback_mode': 'rasaOnly',
                            'irr_source': None,
                            'flags': []
                        }
                    }
                ],
                'expected_behavior': {
                    'should_query_irr': False,
                    'expected_members': [15169, 16591, 19425]
                }
            }
        ]
        
        for case in edge_cases:
            filename = f"edge_{case['name']}.json"
            self._write_fixture(filename, case)
            count += 1
        
        return count
    
    def _write_fixture(self, filename: str, data: Dict[str, Any]):
        filepath = os.path.join(self.output_dir, filename)
        with open(filepath, 'w') as f:
            json.dump(data, f, indent=2)
    
    def generate_multilevel_auth_fixtures(self) -> int:
        count = 0
        
        scenarios = [
            {
                'name': 'auth_level0_only',
                'description': 'AS authorized only at top level',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER1',
                            'containing_as': 2914,
                            'members': [],
                            'nested_sets': ['AS-TIER2'],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    },
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER2',
                            'containing_as': 7922,
                            'members': [],
                            'nested_sets': [],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    }
                ],
                'rasas': [
                    {
                        'rasa': {
                            'authorized_as': 64499,
                            'authorized_in': [
                                {'entry': {'asset': 'AS-TIER1'}, 'propagation': 'unrestricted'}
                            ],
                            'flags': []
                        }
                    }
                ],
                'expected_authorizations': {
                    'AS-TIER1': {'64499': 'authorized'},
                    'AS-TIER2': {'64499': 'not_authorized'}
                }
            },
            {
                'name': 'auth_all_levels',
                'description': 'AS authorized at all levels',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER1',
                            'containing_as': 2914,
                            'members': [],
                            'nested_sets': ['AS-TIER2'],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    },
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER2',
                            'containing_as': 7922,
                            'members': [],
                            'nested_sets': ['AS-TIER3'],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    },
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER3',
                            'containing_as': 11039,
                            'members': [],
                            'nested_sets': [],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    }
                ],
                'rasas': [
                    {
                        'rasa': {
                            'authorized_as': 64499,
                            'authorized_in': [
                                {'entry': {'asset': 'AS-TIER1'}, 'propagation': 'unrestricted'},
                                {'entry': {'asset': 'AS-TIER2'}, 'propagation': 'unrestricted'},
                                {'entry': {'asset': 'AS-TIER3'}, 'propagation': 'unrestricted'}
                            ],
                            'flags': []
                        }
                    }
                ],
                'expected_authorizations': {
                    'AS-TIER1': {'64499': 'authorized'},
                    'AS-TIER2': {'64499': 'authorized'},
                    'AS-TIER3': {'64499': 'authorized'}
                }
            },
            {
                'name': 'auth_direct_only',
                'description': 'AS authorized with directOnly propagation',
                'rasa_sets': [
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER1',
                            'containing_as': 2914,
                            'members': [64499],
                            'nested_sets': ['AS-TIER2'],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    },
                    {
                        'rasa_set': {
                            'as_set_name': 'AS-TIER2',
                            'containing_as': 7922,
                            'members': [64500],
                            'nested_sets': [],
                            'fallback_mode': 'irrFallback',
                            'flags': []
                        }
                    }
                ],
                'rasas': [
                    {
                        'rasa': {
                            'authorized_as': 64499,
                            'authorized_in': [
                                {'entry': {'asset': 'AS-TIER1'}, 'propagation': 'directOnly'}
                            ],
                            'flags': []
                        }
                    }
                ],
                'expected_authorizations': {
                    'AS-TIER1': {'64499': 'authorized'},
                    'AS-TIER2': {'64499': 'not_authorized_direct_only'}
                }
            }
        ]
        
        for scenario in scenarios:
            filename = f"multilevel_{scenario['name']}.json"
            self._write_fixture(filename, scenario)
            count += 1
        
        return count


def main():
    import argparse
    parser = argparse.ArgumentParser(description='Generate RASA test fixtures')
    parser.add_argument('-o', '--output', default='../fixtures',
                       help='Output directory for fixtures')
    parser.add_argument('--count', action='store_true',
                       help='Count fixtures and exit')
    parser.add_argument('--limit', type=int, default=None,
                       help='Limit number of fixtures per category')
    args = parser.parse_args()
    
    generator = FixtureGenerator(args.output)
    
    if args.count:
        counts = generator.engine.count_valid_combinations()
        print(f"Estimated fixture count:")
        print(f"  Fallback mode: {counts['fallback_mode_tests']}")
        print(f"  Hierarchy: {counts['hierarchy_tests']}")
        print(f"  ASN ranges: {counts['asn_range_tests']}")
        print(f"  IRR sources: {counts['irr_source_tests']}")
        print(f"  RASA-AUTH: {counts['rasa_auth_tests']}")
        print(f"  Combined: {counts['combined_tests']}")
        print(f"  Boundary: {counts['boundary_tests']}")
        print(f"  Edge cases: 5+")
        print(f"  Multi-level auth: 3+")
        print(f"  TOTAL: {counts['total']}")
        return
    
    print("Generating RASA test fixtures...")
    counts = generator.generate_all_fixtures()
    
    auth_count = generator.generate_multilevel_auth_fixtures()
    counts['multilevel_auth'] = auth_count
    counts['total'] += auth_count
    
    print(f"\nGenerated {counts['total']} fixtures:")
    print(f"  Fallback mode: {counts['fallback_mode']}")
    print(f"  Hierarchy: {counts['hierarchy']}")
    print(f"  ASN ranges: {counts.get('asn_range', 0)}")
    print(f"  IRR sources: {counts.get('irr_source', 0)}")
    print(f"  RASA-AUTH: {counts.get('rasa_auth', 0)}")
    print(f"  Combined: {counts.get('combined', 0)}")
    print(f"  Boundary: {counts.get('boundary', 0)}")
    print(f"  Edge cases: {counts['edge_cases']}")
    print(f"  Multi-level auth: {auth_count}")
    print(f"\nOutput directory: {args.output}")


if __name__ == '__main__':
    main()
