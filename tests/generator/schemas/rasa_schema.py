#!/usr/bin/env python3
from typing import Dict, List, Any, Optional
from dataclasses import dataclass
from enum import Enum


from typing import Dict, List, Any, Optional
from dataclasses import dataclass
from enum import Enum


class FallbackMode(Enum):
    IRR_FALLBACK = "irrFallback"
    IRR_LOCK = "irrLock"
    RASA_ONLY = "rasaOnly"


class RasaSetFlag(Enum):
    NONE = "none"
    DO_NOT_INHERIT = "doNotInherit"
    AUTHORITATIVE = "authoritative"
    DO_NOT_INHERIT_AUTHORITATIVE = "doNotInherit+authoritative"


class RasaAuthFlag(Enum):
    NONE = "none"
    STRICT_MODE = "strictMode"


class PropagationScope(Enum):
    UNRESTRICTED = "unrestricted"
    DIRECT_ONLY = "directOnly"


class VendorFormat(Enum):
    CISCO = "cisco"
    CISCO_XR = "cisco_xr"
    JUNOS = "junos"
    BIRD = "bird"
    OPENBGPD = "openbgpd"
    JSON = "json"
    NOKIA = "nokia"
    NOKIA_MD = "nokia_md"
    NOKIA_SRL = "nokia_srl"
    HUAWEI = "huawei"
    HUAWEI_XPL = "huawei_xpl"
    ARISTA = "arista"
    MIKROTIK6 = "mikrotik6"
    MIKROTIK7 = "mikrotik7"


class GenerationType(Enum):
    PREFIX_LIST = "prefix_list"
    AS_PATH = "as_path"
    AS_SET = "as_set"
    EACL = "eacl"
    ROUTE_FILTER = "route_filter"


RASA_SCHEMA = {
    'fallback_modes': [
        FallbackMode.IRR_FALLBACK,
        FallbackMode.IRR_LOCK,
        FallbackMode.RASA_ONLY
    ],
    
    'hierarchy_levels': [1, 2, 3],
    
    'rasa_configurations': [
        'none',
        'auth_only',
        'set_only',
        'both'
    ],
    
    'rasa_set_flags': [
        RasaSetFlag.NONE,
        RasaSetFlag.DO_NOT_INHERIT,
        RasaSetFlag.AUTHORITATIVE,
        RasaSetFlag.DO_NOT_INHERIT_AUTHORITATIVE
    ],
    
    'rasa_auth_flags': [
        RasaAuthFlag.NONE,
        RasaAuthFlag.STRICT_MODE
    ],
    
    'propagation_scopes': [
        PropagationScope.UNRESTRICTED,
        PropagationScope.DIRECT_ONLY
    ],
    
    'vendor_formats': [
        VendorFormat.CISCO,
        VendorFormat.CISCO_XR,
        VendorFormat.JUNOS,
        VendorFormat.BIRD,
        VendorFormat.OPENBGPD,
        VendorFormat.JSON,
        VendorFormat.NOKIA,
        VendorFormat.HUAWEI
    ],
    
    'generation_types': [
        GenerationType.PREFIX_LIST,
        GenerationType.AS_PATH,
        GenerationType.AS_SET
    ]
}


@dataclass
class RasaSetConfig:
    as_set_name: str
    containing_as: int
    members: List[int]
    nested_sets: List[str]
    fallback_mode: FallbackMode
    irr_source: Optional[str] = None
    flags: RasaSetFlag = RasaSetFlag.NONE
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'as_set_name': self.as_set_name,
            'containing_as': self.containing_as,
            'members': self.members,
            'nested_sets': self.nested_sets,
            'fallback_mode': self.fallback_mode.value,
            'irr_source': self.irr_source,
            'flags': self.flags.value
        }


@dataclass
class AuthorizedEntry:
    as_set_name: str
    propagation: PropagationScope = PropagationScope.UNRESTRICTED
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'as_set_name': self.as_set_name,
            'propagation': self.propagation.value
        }


@dataclass
class RasaAuthConfig:
    authorized_as: int
    authorized_in: List[AuthorizedEntry]
    flags: RasaAuthFlag = RasaAuthFlag.NONE
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'authorized_as': self.authorized_as,
            'authorized_in': [e.to_dict() for e in self.authorized_in],
            'flags': self.flags.value
        }


@dataclass
class TestCase:
    name: str
    test_type: str
    description: str
    rasa_set: Optional[RasaSetConfig] = None
    rasa_auth: Optional[RasaAuthConfig] = None
    hierarchy_levels: int = 1
    expected_result: Dict[str, Any] = None
    
    def to_dict(self) -> Dict[str, Any]:
        return {
            'name': self.name,
            'test_type': self.test_type,
            'description': self.description,
            'rasa_set': self.rasa_set.to_dict() if self.rasa_set else None,
            'rasa_auth': self.rasa_auth.to_dict() if self.rasa_auth else None,
            'hierarchy_levels': self.hierarchy_levels,
            'expected_result': self.expected_result or {}
        }


def get_schema() -> Dict[str, Any]:
    return RASA_SCHEMA


def validate_config(config: Dict[str, Any]) -> bool:
    if config.get('fallback_mode') == FallbackMode.IRR_LOCK.value:
        if not config.get('irr_source'):
            return False

    
    if config.get('fallback_mode') == FallbackMode.RASA_ONLY.value:
        if not config.get('members'):
            return False
    
    return True


if __name__ == '__main__':
    print("RASA Test Schema")
    print("================")
    for key, values in RASA_SCHEMA.items():
        print(f"\n{key}:")
        for v in values:
            if isinstance(v, Enum):
                print(f"  - {v.value}")
            else:
                print(f"  - {v}")
