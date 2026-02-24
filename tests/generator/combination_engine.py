#!/usr/bin/env python3
from typing import Dict, List, Any, Optional, Iterator
from schemas.rasa_schema import (
    FallbackMode, RasaSetFlag, RasaAuthFlag, PropagationScope,
    RasaSetConfig, RasaAuthConfig, AuthorizedEntry
)

class CombinationEngine:
    def __init__(self):
        self.invalid_combinations = self._define_invalid_combinations()
    
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
        
        from itertools import product
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
    
    def count_valid_combinations(self) -> Dict[str, int]:
        counts = {
            'fallback_mode_tests': 0,
            'hierarchy_tests': 0,
            'total': 0
        }
        
        for _ in self.generate_all_fallback_mode_tests():
            counts['fallback_mode_tests'] += 1
        
        for levels in [1, 2, 3]:
            for fallback_mode in FallbackMode:
                for _ in self.generate_hierarchy_tests(levels, fallback_mode):
                    counts['hierarchy_tests'] += 1
        
        counts['total'] = counts['fallback_mode_tests'] + counts['hierarchy_tests']
        
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
