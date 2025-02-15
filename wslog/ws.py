import os
import sys
import websockets
import asyncio

async def wslog(websocket):
    print("New connection from", websocket.remote_address)
    while True:
        try:
            message = await websocket.recv()
            print(f"{websocket.remote_address[0]} : {message}")
        except websockets.exceptions.ConnectionClosedOK:
            print(f"Connection closed by {websocket.remote_address}")
            break

async def main():
    async with websockets.serve(wslog, "0.0.0.0", 8080):
        await asyncio.Future()

if __name__ == "__main__":
    asyncio.run(main())
