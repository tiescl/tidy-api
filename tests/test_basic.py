import datetime


from testsuite.daemons.service_client import Client


async def test_basic(service_client: Client):
    response = await service_client.get('/hello', params={'name': 'Tester'})
    assert response.status == 200

    text = response.text
    assert text.startswith('Hello from ')
    assert text.endswith(', Tester!\n')

    timestamp_str = text.replace('Hello from ', '').replace(', Tester!\n', '')
    timestamp = datetime.datetime.fromisoformat(timestamp_str)

    now = datetime.datetime.now(datetime.timezone.utc)

    delta = abs((now - timestamp).total_seconds())
    assert delta < 1.0, f"timestamp delta too large: {delta} seconds"
