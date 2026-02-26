#!/usr/bin/env python3
from typing import Dict, List, Any, Optional, Iterator, Tuple
from itertools import product
from schemas.rasa_schema import (
    FallbackMode, RasaSetFlag, RasaAuthFlag, PropagationScope,
    RasaSetConfig, RasaAuthConfig, AuthorizedEntry
)

class CombinationEngine:
    def __init__(self):
        self.invalid_combinations = self._define_invalid_combinations()
        # ASN ranges for comprehensive testing
        self.asn_ranges = {
            '16bit_private': (64512, 65534),
            '16bit_public': (1, 64495),
            '16bit_documentation': (64496, 64511),
            '32bit_private': (4200000000, 4294967294),
            '32bit_public': (65536, 4199999999),
            'real_world': [
                (6939, 6939),    # Hurricane Electric
                (15169, 15169),  # Google
                (2914, 2914),    # NTT
                (1299, 1299),    # Arelion
                (6461, 6461),    # Zayo
                (7922, 7922),    # Comcast
                (16591, 16591),  # Google Fiber
                (19425, 19425),  # Google Asia
                (11039, 11039),  # Comcast
                (3320, 3320),    # DTAG
                (3356, 3356),    # Lumen
                (174, 174),      # Cogent
                (6762, 6762),    # Telecom Italia
                (3257, 3257),    # GTT
                (1273, 1273),    # CW
                (6830, 6830),    # Liberty Global
                (3491, 3491),    # PCCW
                (5511, 5511),    # Orange
            ]
        }
        
        self.irr_sources = ['RADB', 'RIPE', 'APNIC', 'ARIN', 'LACNIC', 'AFRINIC', 
                           'ALTDB', 'BELL', 'LEVEL3', 'NTTCOM', 'JPIRR', 'TC']
    
    def _define_invalid_combinations(self) -> List[Dict[str, Any]]:
        return [
            {'fallback_mode': FallbackMode.IRR_LOCK, 'has_irr_source': False},
            {'fallback_mode': FallbackMode.RASA_ONLY, 'members': []},
        ]
    
    def generate_rasa_set_config(
        self,
        as_set_name: str,
        fallback_mode: FallbackMode,
        flags: RasaSetFlag,
        containing_as: int = 64496,
        members: Optional[List[int]] = None,
        nested_sets: Optional[List[str]] = None,
        irr_source: Optional[str] = None
    ) -> Optional[RasaSetConfig]:
        if not self._is_valid_combination(fallback_mode, irr_source, members):
            return None
        
        if members is None:
            members = [64497, 64498] if fallback_mode == FallbackMode.RASA_ONLY else []
        
        if nested_sets is None:
            nested_sets = []
        
        if fallback_mode == FallbackMode.IRR_LOCK and not irr_source:
            irr_source = "RADB"
        
        return RasaSetConfig(
            as_set_name=as_set_name,
            containing_as=containing_as,
            members=members,
            nested_sets=nested_sets,
            fallback_mode=fallback_mode,
            irr_source=irr_source,
            flags=flags
        )
    
    def _is_valid_combination(
        self,
        fallback_mode: FallbackMode,
        irr_source: Optional[str],
        members: Optional[List[int]]
    ) -> bool:
        if fallback_mode == FallbackMode.IRR_LOCK and not irr_source:
            return False
        
        if fallback_mode == FallbackMode.RASA_ONLY:
            if not members or len(members) == 0:
                return False
        
        return True
    
    def generate_hierarchy(
        self,
        levels: int,
        fallback_mode: FallbackMode,
        flags_per_level: Dict[int, RasaSetFlag],
        base_asn: int = 64496
    ) -> List[RasaSetConfig]:
        hierarchy = []
        
        for level in range(levels):
            as_set_name = f"AS-LEVEL{level}"
            containing_as = base_asn + level
            members = [base_asn + 100 + level]
            
            nested_sets = []
            if level < levels - 1:
                nested_sets = [f"AS-LEVEL{level+1}"]
            
            flags = flags_per_level.get(level, RasaSetFlag.NONE)
            
            rasa_set = self.generate_rasa_set_config(
                as_set_name=as_set_name,
                fallback_mode=fallback_mode,
                flags=flags,
                containing_as=containing_as,
                members=members,
                nested_sets=nested_sets
            )
            
            if rasa_set:
                hierarchy.append(rasa_set)
        
        return hierarchy
    
    def calculate_expected_behavior(self, config: RasaSetConfig) -> Dict[str, Any]:
        behavior = {
            'should_query_irr': True,
            'irr_source': None,
            'expected_members': [],
            'expand_nested': True,
            'warnings': [],
            'errors': []
        }
        
        if config.fallback_mode == FallbackMode.IRR_LOCK:
            behavior['irr_source'] = config.irr_source
        elif config.fallback_mode == FallbackMode.RASA_ONLY:
            behavior['should_query_irr'] = False
            behavior['expected_members'] = config.members
        else:
            behavior['expected_members'] = config.members
        
        if config.flags in [RasaSetFlag.DO_NOT_INHERIT, RasaSetFlag.DO_NOT_INHERIT_AUTHORITATIVE]:
            behavior['expand_nested'] = False
        
        return behavior
    
    def generate_all_fallback_mode_tests(self) -> Iterator[Dict[str, Any]]:
        test_id = 0
        
        for fallback_mode in FallbackMode:
            for flags in RasaSetFlag:
                for has_irr_source in [True, False]:
                    for has_members in [True, False]:
                        test_id += 1
                        
                        members = [64497, 64498] if has_members else []
                        irr_source = "RADB" if has_irr_source else None
                        
                        config = self.generate_rasa_set_config(
                            as_set_name=f"AS-TEST-{test_id}",
                            fallback_mode=fallback_mode,
                            flags=flags,
                            members=members,
                            irr_source=irr_source
                        )
                        
                        if config:
                            expected = self.calculate_expected_behavior(config)
                            
                            yield {
                                'name': f'fallback_{fallback_mode.value}_{flags.value}_{test_id}',
                                'type': 'fallback_mode',
                                'config': config,
                                'expected': expected,
                                'should_succeed': True
                            }
    
    def generate_hierarchy_tests(
        self,
        levels: int,
        fallback_mode: FallbackMode
    ) -> Iterator[Dict[str, Any]]:
        test_id = 0
        
        # More comprehensive flag combinations including all flag types
        flag_options = [RasaSetFlag.NONE, RasaSetFlag.DO_NOT_INHERIT, 
                       RasaSetFlag.AUTHORITATIVE, RasaSetFlag.DO_NOT_INHERIT_AUTHORITATIVE]
        
        # For higher levels, limit combinations to avoid explosion
        if levels <= 3:
            flag_combinations = list(product(flag_options, repeat=levels))
        else:
            # For 4-5 levels, use simpler combinations
            flag_combinations = list(product([RasaSetFlag.NONE, RasaSetFlag.DO_NOT_INHERIT], repeat=levels))
        
        for flag_combo in flag_combinations:
            test_id += 1
            flags_per_level = {i: flag_combo[i] for i in range(levels)}
            
            hierarchy = self.generate_hierarchy(
                levels=levels,
                fallback_mode=fallback_mode,
                flags_per_level=flags_per_level
            )
            
            if hierarchy:
                expected_expansion = self._calculate_hierarchy_expansion(hierarchy, flags_per_level)
                
                yield {
                    'name': f'hierarchy_{levels}level_{fallback_mode.value}_{test_id}',
                    'type': 'hierarchy',
                    'levels': levels,
                    'fallback_mode': fallback_mode,
                    'flags_per_level': flags_per_level,
                    'hierarchy': hierarchy,
                    'expected_expansion': expected_expansion
                }
    
    def _calculate_hierarchy_expansion(
        self,
        hierarchy: List[RasaSetConfig],
        flags_per_level: Dict[int, RasaSetFlag]
    ) -> int:
        for level in range(len(hierarchy)):
            flags = flags_per_level.get(level, RasaSetFlag.NONE)
            if flags in [RasaSetFlag.DO_NOT_INHERIT, RasaSetFlag.DO_NOT_INHERIT_AUTHORITATIVE]:
                return level + 1
        
        return len(hierarchy)
    
    def generate_asn_range_tests(self) -> Iterator[Dict[str, Any]]:
        test_id = 0
        set_flags = [RasaSetFlag.NONE, RasaSetFlag.AUTHORITATIVE, 
                     RasaSetFlag.DO_NOT_INHERIT, RasaSetFlag.DO_NOT_INHERIT_AUTHORITATIVE]
        
        for range_name, range_val in self.asn_ranges.items():
            if isinstance(range_val, list):
                # Real-world ASNs - test each with all fallback modes and flags
                for asn_start, asn_end in range_val:
                    asn = asn_start
                    for fallback_mode in FallbackMode:
                        for flags in set_flags:
                            test_id += 1
                            members = [asn + 1, asn + 2] if fallback_mode == FallbackMode.RASA_ONLY else []
                            irr_source = "RADB" if fallback_mode == FallbackMode.IRR_LOCK else None
                            
                            config = self.generate_rasa_set_config(
                                as_set_name=f"AS-{range_name.upper()}-{asn}-{test_id}",
                                fallback_mode=fallback_mode,
                                flags=flags,
                                containing_as=asn,
                                members=members,
                                irr_source=irr_source
                            )
                            
                            if config:
                                expected = self.calculate_expected_behavior(config)
                                yield {
                                    'name': f'asn_{range_name}_{asn}_{fallback_mode.value}_{flags.value}_{test_id}',
                                    'type': 'asn_range',
                                    'asn_range': range_name,
                                    'config': config,
                                    'expected': expected,
                                    'should_succeed': True
                                }
            else:
                # Generate comprehensive samples from range
                asn_start, asn_end = range_val
                # Generate 100 samples per range
                step = max(1, (asn_end - asn_start) // 100)
                samples = 0
                for asn in range(asn_start, asn_end + 1, step):
                    if samples >= 100:
                        break
                    samples += 1
                    for fallback_mode in FallbackMode:
                        for flags in set_flags:
                            test_id += 1
                            members = [asn + 1, asn + 2] if fallback_mode == FallbackMode.RASA_ONLY else []
                            irr_source = "RADB" if fallback_mode == FallbackMode.IRR_LOCK else None
                            
                            config = self.generate_rasa_set_config(
                                as_set_name=f"AS-{range_name.upper()}-{asn}",
                                fallback_mode=fallback_mode,
                                flags=flags,
                                containing_as=asn,
                                members=members,
                                irr_source=irr_source
                            )
                            
                            if config:
                                expected = self.calculate_expected_behavior(config)
                                yield {
                                    'name': f'asn_{range_name}_{asn}_{fallback_mode.value}_{flags.value}_{test_id}',
                                    'type': 'asn_range',
                                    'asn_range': range_name,
                                    'config': config,
                                    'expected': expected,
                                    'should_succeed': True
                                }
    
    def generate_irr_source_tests(self) -> Iterator[Dict[str, Any]]:
        test_id = 0
        for irr_source in self.irr_sources:
            for fallback_mode in [FallbackMode.IRR_FALLBACK, FallbackMode.IRR_LOCK]:
                for flags in RasaSetFlag:
                    test_id += 1
                    
                    config = self.generate_rasa_set_config(
                        as_set_name=f"AS-IRR-{irr_source}-{test_id}",
                        fallback_mode=fallback_mode,
                        flags=flags,
                        containing_as=64496 + test_id,
                        members=[],
                        irr_source=irr_source
                    )
                    
                    if config:
                        expected = self.calculate_expected_behavior(config)
                        yield {
                            'name': f'irr_source_{irr_source.lower()}_{fallback_mode.value}_{flags.value}_{test_id}',
                            'type': 'irr_source',
                            'irr_source': irr_source,
                            'config': config,
                            'expected': expected,
                            'should_succeed': True
                        }
    
    def generate_rasa_auth_tests(self) -> Iterator[Dict[str, Any]]:
        test_id = 0
        auth_flags = [RasaAuthFlag.NONE, RasaAuthFlag.STRICT_MODE]
        propagation_scopes = [PropagationScope.UNRESTRICTED, PropagationScope.DIRECT_ONLY]
        
        # More comprehensive ASN ranges for authorization tests
        auth_asns = list(range(64496, 64596)) + list(range(4200000000, 4200000100))
        
        for auth_flag in auth_flags:
            for propagation in propagation_scopes:
                for num_authorized_assets in [1, 2, 3, 5, 10]:
                    for authorized_as in auth_asns[:50]:  # Test 50 different ASNs
                        test_id += 1
                        authorized_in = []
                        for i in range(num_authorized_assets):
                            authorized_in.append(
                                AuthorizedEntry(
                                    as_set_name=f"AS-AUTH-{i}",
                                    propagation=propagation
                                )
                            )
                        
                        auth_config = RasaAuthConfig(
                            authorized_as=authorized_as,
                            authorized_in=authorized_in,
                            flags=auth_flag
                        )
                        
                        yield {
                            'name': f'rasa_auth_{auth_flag.value}_{propagation.value}_{num_authorized_assets}assets_asn{authorized_as}_{test_id}',
                            'type': 'rasa_auth',
                            'auth_flag': auth_flag.value,
                            'propagation': propagation.value,
                            'auth_config': auth_config,
                            'expected_authorized': num_authorized_assets,
                            'should_succeed': True
                        }
    
    def generate_combined_tests(self) -> Iterator[Dict[str, Any]]:
        test_id = 0
        # Combined RASA-SET + RASA-AUTH tests
        set_fallback_modes = [FallbackMode.IRR_FALLBACK, FallbackMode.IRR_LOCK, FallbackMode.RASA_ONLY]
        auth_flags = [RasaAuthFlag.NONE, RasaAuthFlag.STRICT_MODE]
        set_flags = [RasaSetFlag.NONE, RasaSetFlag.AUTHORITATIVE, 
                     RasaSetFlag.DO_NOT_INHERIT, RasaSetFlag.DO_NOT_INHERIT_AUTHORITATIVE]
        propagation_scopes = [PropagationScope.UNRESTRICTED, PropagationScope.DIRECT_ONLY]
        
        # Generate comprehensive combined tests
        for set_fallback in set_fallback_modes:
            for auth_flag in auth_flags:
                for set_flag in set_flags:
                    for propagation in propagation_scopes:
                        for num_authored in [1, 2, 3]:
                            test_id += 1
                            members = [64497, 64498, 64499]
                            irr_source = "RADB" if set_fallback == FallbackMode.IRR_LOCK else None
                            
                            set_config = self.generate_rasa_set_config(
                                as_set_name=f"AS-COMBINED-{test_id}",
                                fallback_mode=set_fallback,
                                flags=set_flag,
                                containing_as=64496,
                                members=members,
                                irr_source=irr_source
                            )
                            
                            authorized_in = []
                            for i in range(num_authored):
                                authorized_in.append(
                                    AuthorizedEntry(
                                        as_set_name=f"AS-COMBINED-{test_id}",
                                        propagation=propagation
                                    )
                                )
                            
                            auth_config = RasaAuthConfig(
                                authorized_as=64497,
                                authorized_in=authorized_in,
                                flags=auth_flag
                            )
                            
                            if set_config:
                                yield {
                                    'name': f'combined_{set_fallback.value}_{set_flag.value}_{auth_flag.value}_{propagation.value}_{num_authored}auth_{test_id}',
                                    'type': 'combined',
                                    'set_config': set_config,
                                    'auth_config': auth_config,
                                    'should_succeed': True
                                }
    
    def generate_boundary_tests(self) -> Iterator[Dict[str, Any]]:
        """Generate tests for boundary values and edge cases"""
        test_id = 0
        
        # ASN boundaries
        boundary_asns = [
            (1, "min_public"),
            (64495, "max_16bit_public"),
            (64496, "min_doc"),
            (64511, "max_doc"),
            (64512, "min_private"),
            (65534, "max_private"),
            (65535, "reserved"),
            (65536, "min_32bit"),
            (4199999999, "max_32bit_public"),
            (4200000000, "min_32bit_private"),
            (4294967294, "max_32bit_private"),
        ]
        
        for asn, boundary_type in boundary_asns:
            for fallback_mode in FallbackMode:
                test_id += 1
                members = [asn + 1] if fallback_mode == FallbackMode.RASA_ONLY else []
                irr_source = "RADB" if fallback_mode == FallbackMode.IRR_LOCK else None
                
                config = self.generate_rasa_set_config(
                    as_set_name=f"AS-BOUNDARY-{boundary_type.upper()}",
                    fallback_mode=fallback_mode,
                    flags=RasaSetFlag.NONE,
                    containing_as=asn,
                    members=members,
                    irr_source=irr_source
                )
                
                if config:
                    expected = self.calculate_expected_behavior(config)
                    yield {
                        'name': f'boundary_{boundary_type}_{fallback_mode.value}_{test_id}',
                        'type': 'boundary',
                        'boundary_type': boundary_type,
                        'asn': asn,
                        'config': config,
                        'expected': expected,
                        'should_succeed': True
                    }
        
        # Empty/invalid combinations that should fail
        invalid_configs = [
            {'name': 'empty_members_rasaonly', 'fallback_mode': FallbackMode.RASA_ONLY, 'members': [], 'containing_as': 64496},
            {'name': 'no_irrsource_irrlock', 'fallback_mode': FallbackMode.IRR_LOCK, 'members': [], 'containing_as': 64496, 'irr_source': None},
        ]
        
        for invalid in invalid_configs:
            test_id += 1
            yield {
                'name': f"invalid_{invalid['name']}_{test_id}",
                'type': 'invalid',
                'config': invalid,
                'should_succeed': False,
                'expected_error': f"Invalid configuration: {invalid['name']}"
            }
    
    def count_valid_combinations(self) -> Dict[str, int]:
        counts = {
            'fallback_mode_tests': 0,
            'hierarchy_tests': 0,
            'asn_range_tests': 0,
            'irr_source_tests': 0,
            'rasa_auth_tests': 0,
            'combined_tests': 0,
            'boundary_tests': 0,
            'total': 0
        }
        
        for _ in self.generate_all_fallback_mode_tests():
            counts['fallback_mode_tests'] += 1
        
        for levels in [1, 2, 3, 4, 5]:
            for fallback_mode in FallbackMode:
                for _ in self.generate_hierarchy_tests(levels, fallback_mode):
                    counts['hierarchy_tests'] += 1
        
        for _ in self.generate_asn_range_tests():
            counts['asn_range_tests'] += 1
        
        for _ in self.generate_irr_source_tests():
            counts['irr_source_tests'] += 1
        
        for _ in self.generate_rasa_auth_tests():
            counts['rasa_auth_tests'] += 1
        
        for _ in self.generate_combined_tests():
            counts['combined_tests'] += 1
        
        for _ in self.generate_boundary_tests():
            counts['boundary_tests'] += 1
        
        counts['total'] = sum(counts.values())
        
        return counts


if __name__ == '__main__':
    engine = CombinationEngine()
    
    print("Validating combination engine...")
    counts = engine.count_valid_combinations()
    print(f"\nTest combination counts:")
    print(f"  Fallback mode tests: {counts['fallback_mode_tests']}")
    print(f"  Hierarchy tests: {counts['hierarchy_tests']}")
    print(f"  Total: {counts['total']}")
    
    print("\n\nSample fallback mode tests:")
    for i, test in enumerate(engine.generate_all_fallback_mode_tests()):
        if i >= 3:
            break
        print(f"\n{test['name']}:")
        print(f"  Fallback: {test['config'].fallback_mode.value}")
        print(f"  Flags: {test['config'].flags.value}")
        print(f"  Query IRR: {test['expected']['should_query_irr']}")
    
    print("\n\nSample hierarchy tests:")
    for i, test in enumerate(engine.generate_hierarchy_tests(2, FallbackMode.IRR_FALLBACK)):
        if i >= 2:
            break
        print(f"\n{test['name']}:")
        print(f"  Levels: {test['levels']}")
        print(f"  Expected expansion: {test['expected_expansion']}")
