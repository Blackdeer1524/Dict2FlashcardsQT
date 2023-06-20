import json
import os
from typing import Literal, TypedDict, Union

import genanki

from .anki_deck_params import *
from .utils import *


def save(deck_path: str):
    if not os.path.exists(deck_path):
        return f"given deck path {deck_path} does not exitst"

    with open(deck_path, "r", encoding="utf-8") as f:
        res = json.load(f)

    anki_deck_name = os.path.basename(deck_path).split(".", 1)[0]
    anki_deck_id = int(str(abs(hash(anki_deck_name)))[:10])
    anki_deck = genanki.Deck(anki_deck_id, anki_deck_name)

    for item in res:
        saving_word = item["word"]
        special = item["special"]
        definition = item["definition"]
        examples = item["examples"]

        audios = item["audios"]
        local_audios = audios["local"]
        web_audios = audios["web"]

        images = item["images"]
        local_images = images["local"]
        web_images = images["web"]

        tags = item["tags"]
        other = item["other"]

        images = " ".join(
            [get_card_image_name(name) for name, info in web_images]
        )
        audios = " ".join(
            [get_card_audio_name(name) for name, info in web_audios]
        )

        sentence_example = " |<br><br>".join(examples)
        note = genanki.Note(
            model=MERGING_RESULTING_MODEL,
            # I have no idea why, but if any of the fields is empty, then card won't be added,
            fields=[
                sentence_example if sentence_example else " ",
                saving_word if saving_word else " ",
                definition if definition else " ",
                images if images else " ",
                audios if audios else " ",
            ],
            tags=tags,
        )
        anki_deck.add_note(note)

    my_package = genanki.Package(anki_deck)
    saving_path = deck_path[: deck_path.find(".")]
    my_package.write_to_file(f"{saving_path}.apkg")
    return ""


def load():
    return


def unload():
    return


def get_config_description():
    return {}


def get_default_config():
    return {}


class ErrorSummary(TypedDict):
    error_type: Literal[
        "invalid_type", "invalid_value", "empty", "unknown_field"
    ]
    description: str


SetConfigError = dict[str, Union[ErrorSummary, "SetConfigError"]]


def validate_config(new_config: dict) -> SetConfigError:
    res: SetConfigError = {}
    for key in new_config:
        res[key] = {
            "error_type": "unknown_field",
            "description": f"Unknown field: `{key}`",
        }

    return res
