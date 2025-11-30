import typing


def remove_ids_and_timestamps(response: typing.Dict):
    keys_to_remove = ['id', 'created_at', 'updated_at']

    for key in keys_to_remove:
        response.pop(key, None)
