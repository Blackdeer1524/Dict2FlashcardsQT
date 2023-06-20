import os
from typing import Any, Union

from .utils import RESULT_FORMAT, Literal, TypedDict
from .utils import define as _define


def translate(definitons_data: RESULT_FORMAT):
    audio_region_field = "UK_audio_links"
    word_list = []

    for word, pos_lists in definitons_data.items():
        for pos_data in pos_lists:
            pos = pos_data["POS"]
            pos_fields = pos_data["data"]

            for (
                definition,
                definition_translation,
                examples,
                domain,
                level,
                region,
                usage,
                image,
                alt_terms,
                irreg_forms,
                region_audio_links,
            ) in zip(
                pos_fields["definitions"],
                pos_fields["definitions_translations"],
                pos_fields["examples"],
                pos_fields["domains"],
                pos_fields["levels"],
                pos_fields["regions"],
                pos_fields["usages"],
                pos_fields["image_links"],
                pos_fields["alt_terms"],
                pos_fields["irregular_forms"],
                pos_fields[audio_region_field],
            ):  # type: ignore
                current_word_dict = {
                    "word": word.strip(),
                    "special": irreg_forms + alt_terms,
                    "definition": f"{definition_translation}\n{definition}"
                    if definition_translation
                    else definition,
                    "examples": examples,
                    "audios": {
                        "web": [(link, "") for link in region_audio_links],
                        "local": [],
                    },
                    "images": {
                        "web": [(image, "")] if image else [],
                        "local": [],
                    },
                    "other": [],
                    "tags": {
                        "domain": domain,
                        "region": region,
                        "usage": usage,
                        "pos": pos,
                    },
                }
                if level:
                    current_word_dict["tags"]["level"] = level

                word_list.append(current_word_dict)
    return word_list


def get_dict_scheme():
    return {
        "special": {
            "docs": "additional informatio provided",
            "type": "list[string]",
        },
        "tags": {
            "docs": "Dictionary tags",
            "children": {
                "pos": {"docs": "[P]art [O]f [S]peach", "type": "string"},
                "level": {
                    "docs": "English proficiency level. One of [A1, A2, B1, B2, C1, C2]",
                    "type": "string",
                },
                "usage": {
                    "docs": "How current word is used",
                    "type": "list[string]",
                },
                "domain": {
                    "docs": "Domain in which word is used",
                    "type": "list[string]",
                },
                "region": {
                    "docs": "Region where word is used",
                    "type": "list[string]",
                },
            },
        },
    }


def get(word: str):
    batch_size = yield
    definitions, error = _define(
        word,
    )
    cards = translate(definitions)
    i = 0
    while i < len(cards):
        res_batch = cards[i : i + batch_size]
        i += batch_size
        batch_size = yield res_batch, error
    return


def load():
    return


def unload():
    return


ConfigKeys = Literal["audio region", "timeout"]
AUDIO_REGION = "audio region"
TIMEOUT = "timeout"


class ConfigFieldInfo(TypedDict):
    docs: str
    type: str


ConfigDescription = dict[
    ConfigKeys, Union[ConfigFieldInfo, "ConfigDescription"]
]


def get_config_description() -> ConfigDescription:
    return {
        AUDIO_REGION: {
            "docs": "Current audio region",
            "type": "string",
        },
        TIMEOUT: {"docs": "Request timeout", "type": "number"},
    }


def get_default_config() -> dict[ConfigKeys, Any]:
    return {AUDIO_REGION: "us", TIMEOUT: 3}


class ErrorSummary(TypedDict):
    error_type: Literal[
        "invalid_type", "invalid_value", "empty", "unknown_field"
    ]
    description: str


SetConfigError = dict[str, Union[ErrorSummary, "SetConfigError"]]


def validate_config(new_config: dict) -> SetConfigError:
    res: SetConfigError = {}

    if (new_audio_region := new_config.get(AUDIO_REGION)) is not None:
        if type(new_audio_region) != str:
            res[AUDIO_REGION] = {
                "error_type": "invalid_type",
                "description": f"`{AUDIO_REGION}` is expected to be a string",
            }
        elif new_audio_region not in ["uk", "us"]:
            res[AUDIO_REGION] = {
                "error_type": "invalid_value",
                "description": f'`{AUDIO_REGION}` is expected to be one of: ["uk", "us"]',
            }
    else:
        res[AUDIO_REGION] = {
            "error_type": "empty",
            "description": f"`{AUDIO_REGION}` was left empty",
        }

    if (new_timeout := new_config.get(TIMEOUT)) is not None:
        if type(new_timeout) not in [float, int]:
            res[TIMEOUT] = {
                "error_type": "invalid_type",
                "description": f"`{TIMEOUT}` is expected to be a number. Got `{new_timeout}`",
            }
        elif new_timeout < 0:
            res[TIMEOUT] = {
                "error_type": "invalid_value",
                "description": f"`{TIMEOUT}` is expected to be non negative",
            }

    else:
        res[TIMEOUT] = {
            "error_type": "empty",
            "description": "`timeout` was left empty",
        }

    conf_keys = [AUDIO_REGION, TIMEOUT]
    for key in new_config:
        if key in conf_keys:
            continue
        res[key] = {
            "error_type": "unknown_field",
            "description": f"Unknown field: `{key}`",
        }

    return res


if __name__ == "__main__":
    a = get("test")
    next(a)
    qwerwq = a.send(5)
