#!/usr/bin/env python3
import socket
import threading
import json
import re
from typing import Dict, List, Optional, Tuple
from dataclasses import dataclass

IRR_MOCK_DATABASE = {
    'AS-HURRICANE': {
        'members': [
            'AS7922', 'AS11039', 'AS11427', 'AS11588', 'AS11686',
            'AS19151', 'AS20248', 'AS209', 'AS22648', 'AS22995'
        ],
        'sources': ['RADB', 'RIPE', 'ARIN']
    },
    'AS-GOOGLE': {
        'members': [
            'AS15169', 'AS16591', 'AS19425', 'AS19594', 'AS22527'
        ],
        'sources': ['RADB', 'RIPE']
    },
    'AS-AMAZON': {
        'members': [
            'AS16509', 'AS14618', 'AS38895', 'AS62785'
        ],
        'sources': ['RADB', 'ARIN']
    },
    'AS-TIER1': {
        'members': ['AS-HURRICANE', 'AS-GOOGLE', 'AS-TIER2'],
        'sources': ['RADB']
    },
    'AS-TIER2': {
        'members': ['AS64496', 'AS64497', 'AS64498'],
        'sources': ['RADB']
    },
    'AS-LEVEL0': {
        'members': ['AS64500'],
        'sources': ['RADB']
    },
    'AS-LEVEL1': {
        'members': ['AS64501'],
        'sources': ['RADB']
    },
    'AS-LEVEL2': {
        'members': ['AS64502'],
        'sources': ['RADB']
    },
    'AS64496': {
        'routes': ['192.0.2.0/24', '198.51.100.0/24'],
        'sources': ['RADB']
    },
    'AS64497': {
        'routes': ['203.0.113.0/24'],
        'sources': ['RADB']
    }
}


@dataclass
class IRRQuery:
    command: str
    argument: str
    recursive: bool = False


class IRRMockServer:
    def __init__(self, host='127.0.0.1', port=10443):
        self.host = host
        self.port = port
        self.server_socket = None
        self.running = False
        self.current_source = None
        
    def start(self):
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        self.running = True
        
        print(f"IRR Mock Server started on {self.host}:{self.port}")
        
        while self.running:
            try:
                client_socket, address = self.server_socket.accept()
                client_thread = threading.Thread(
                    target=self._handle_client,
                    args=(client_socket, address)
                )
                client_thread.daemon = True
                client_thread.start()
            except Exception as e:
                if self.running:
                    print(f"Error accepting connection: {e}")
    
    def stop(self):
        self.running = False
        if self.server_socket:
            self.server_socket.close()
        print("IRR Mock Server stopped")
    
    def _handle_client(self, client_socket: socket.socket, address):
        try:
            while self.running:
                data = client_socket.recv(4096).decode('utf-8', errors='ignore')
                if not data:
                    break
                
                for line in data.strip().split('\n'):
                    response = self._process_command(line.strip())
                    if response:
                        client_socket.send((response + '\n').encode('utf-8'))
        except Exception as e:
            print(f"Error handling client {address}: {e}")
        finally:
            client_socket.close()
    
    def _process_command(self, command_line: str) -> str:
        if not command_line:
            return ""
        
        if command_line.startswith('!'):
            return self._process_exclamation_command(command_line)
        
        return f"# Unknown command: {command_line}"
    
    def _process_exclamation_command(self, cmd: str) -> str:
        if cmd == '!!':
            return "# Pipeline mode enabled"
        
        if cmd == '!q':
            return "# Quitting"
        
        if cmd.startswith('!s'):
            source = cmd[2:]
            self.current_source = source
            return f"# Source set to {source}"
        
        if cmd.startswith('!t'):
            return "# Timeout set"
        
        if cmd.startswith('!i'):
            return self._handle_member_query(cmd[2:])
        
        if cmd.startswith('!g'):
            return self._handle_route_query(cmd[2:], ipv4=True)
        
        if cmd.startswith('!6'):
            return self._handle_route_query(cmd[2:], ipv4=False)
        
        return f"# Unknown command: {cmd}"
    
    def _handle_member_query(self, arg: str) -> str:
        parts = arg.split(',')
        asset = parts[0]
        recursive = len(parts) > 1 and parts[1] == '1'
        
        data = IRR_MOCK_DATABASE.get(asset)
        if not data:
            return f"# No entries for {asset}"
        
        if self.current_source and self.current_source not in data.get('sources', []):
            return f"# Not in source {self.current_source}"
        
        members = data.get('members', [])
        
        if recursive:
            expanded = self._expand_members(asset, set())
            members = list(expanded)
        
        if not members:
            return f"# No members for {asset}"
        
        response = f"# {len(members)} members\n"
        for member in members:
            response += f"{member}\n"
        return response.strip()
    
    def _expand_members(self, asset: str, seen: set) -> set:
        if asset in seen:
            return set()
        
        seen.add(asset)
        data = IRR_MOCK_DATABASE.get(asset)
        if not data:
            return set()
        
        result = set()
        for member in data.get('members', []):
            if member.startswith('AS') and not member[2:].isdigit():
                result.update(self._expand_members(member, seen))
            else:
                result.add(member)
        
        return result
    
    def _handle_route_query(self, asn: str, ipv4: bool = True) -> str:
        data = IRR_MOCK_DATABASE.get(asn)
        if not data:
            return f"# No routes for {asn}"
        
        routes = data.get('routes', [])
        if not routes:
            return f"# No routes for {asn}"
        
        response = f"# {len(routes)} routes\n"
        for route in routes:
            response += f"{route}\n"
        return response.strip()


class IRRMockClient:
    def __init__(self, host='127.0.0.1', port=10443):
        self.host = host
        self.port = port
        self.socket = None
    
    def connect(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.host, self.port))
    
    def disconnect(self):
        if self.socket:
            self.socket.close()
            self.socket = None
    
    def query(self, command: str) -> str:
        if not self.socket:
            self.connect()
        
        self.socket.send((command + '\n').encode('utf-8'))
        response = self.socket.recv(4096).decode('utf-8')
        return response
    
    def get_members(self, asset: str, recursive: bool = False) -> List[str]:
        cmd = f"!i{asset}"
        if recursive:
            cmd += ",1"
        
        response = self.query(cmd)
        lines = response.strip().split('\n')
        
        members = []
        for line in lines:
            if line.startswith('#') or not line.strip():
                continue
            members.append(line.strip())
        
        return members
    
    def set_source(self, source: str):
        self.query(f"!s{source}")


def start_mock_server(host='127.0.0.1', port=10443):
    server = IRRMockServer(host, port)
    
    def run_server():
        server.start()
    
    thread = threading.Thread(target=run_server)
    thread.daemon = True
    thread.start()
    
    return server


if __name__ == '__main__':
    import sys
    import time
    
    server = IRRMockServer()
    
    def signal_handler(sig, frame):
        print("\nShutting down...")
        server.stop()
        sys.exit(0)
    
    import signal
    signal.signal(signal.SIGINT, signal_handler)
    
    print("Starting IRR Mock Server...")
    print("Press Ctrl+C to stop")
    server.start()
